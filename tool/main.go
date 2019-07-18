package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"text/template"

	"gopkg.in/yaml.v2"
)

var (
	projectRoot = flag.String("R", ".", "project root src dir")
	statedef    = flag.String("s", "", "yaml of states")
)

type State struct {
	Name        string
	ClassName   string
	FileName    string // 文件名，不包括目录和后缀
	Description string
	Timeout     int
	Wait        bool
	Path        string // 要生成的代码的相对目录
	Signals     []string
}

func main() {

	flag.Parse()

	// fmt.Println(*statedef)

	f, err := os.Open(*statedef)
	if err != nil {
		panic(err)
	}

	states := []State{}
	dec := yaml.NewDecoder(f)
	for {
		s := struct{ State State }{}
		s.State.Wait = true
		err = dec.Decode(&s)
		if err == io.EOF {
			// log.Println(err)
			break
		}
		if err != nil {
			panic(err)
		}
		s.State.ClassName = fmt.Sprintf("State%s", s.State.Name)
		s.State.FileName = fmt.Sprintf("state_%s", s.State.Name)
		if len(s.State.Path) == 0 {
			s.State.Path = "."
		}
		// 默认超时10秒
		s.State.Timeout = 10000
		log.Printf("%#v\n", s.State)
		states = append(states, s.State)
	}
	// log.Printf("%v\n", states)

	fooH, err := template.New(".h").Parse(FooH)
	if err != nil {
		panic(err)
	}
	fooCpp, err := template.New(".cpp").Parse(FooCpp)
	if err != nil {
		panic(err)
	}
	fooImplCpp, err := template.New("_impl.cpp").Parse(FooImplCpp)
	if err != nil {
		panic(err)
	}

	// 对每个state写3个文件
	for _, state := range states {
		err = writeStateFile(fooH, &state, true)
		if err != nil {
			panic(err)
		}

		err = writeStateFile(fooCpp, &state, true)
		if err != nil {
			panic(err)
		}

		dir := filepath.Join(*projectRoot, "states", "impl", state.Path)
		err = os.MkdirAll(dir, 0755)
		if err != nil {
			log.Fatal(err)
		}
		p := filepath.Join(dir, fmt.Sprintf("%s_impl.cpp", state.FileName))
		_, err = os.Stat(p)
		if os.IsNotExist(err) {
			f, err := os.Create(p)
			if err != nil {
				panic(err)
			}
			err = fooImplCpp.Execute(f, state)
			if err != nil {
				panic(err)
			}
		} else if err != nil {
			panic(err)
		}
	}

	// 生成一个CMakeLists.txt
	tpl, err := template.New("CMakeLists.txt").Parse(CMakeTPL)
	if err != nil {
		panic(err)
	}
	f, err = os.Create(filepath.Join(*projectRoot, "states", "CMakeLists.txt"))
	if err != nil {
		panic(err)
	}
	err = tpl.Execute(f, &states)
	if err != nil {
		panic(err)
	}

	// 生成一个 state_factory
	tpl, err = template.New("state_factory.cpp").Parse(StateFactoryTPL)
	if err != nil {
		panic(err)
	}
	f, err = os.Create(filepath.Join(*projectRoot, "states", "autogen", "state_factory.cpp"))
	if err != nil {
		panic(err)
	}
	err = tpl.Execute(f, &states)
	if err != nil {
		panic(err)
	}
}

func writeStateFile(tpl *template.Template, state *State, overwrite bool) error {
	// 确保dir存在
	dir := filepath.Join(*projectRoot, "states", "autogen", state.Path)
	err := os.MkdirAll(dir, 0775)
	if err != nil {
		return err
	}

	p := filepath.Join(dir, fmt.Sprintf("%s%s", state.FileName, tpl.Name()))
	_, err = os.Stat(p)
	if !overwrite && !os.IsNotExist(err) {
		return nil
	}

	f, err := os.Create(p)
	if err != nil {
		return err
	}
	defer f.Close()

	err = tpl.Execute(f, state)
	if err != nil {
		return err
	}

	return nil
}

const FooH = `
#pragma once
#include <testcase/basic_state.h>

namespace state {

// 状态描述：{{ .Description }}
class {{ .ClassName }} : public BasicState {
    Q_OBJECT
public:
	static {{ .ClassName }}* create(QState* parent);
    explicit {{ .ClassName }}(QState* parent = nullptr);
protected:
	void perform() override {}
	void clean() override {}
Q_SIGNALS:
    {{ range .Signals }}
    void ev_{{ . }}();
    {{ end }}
};

}
`

const FooCpp = `
#include "{{ .FileName }}.h"

namespace state {
    {{ .ClassName }}::{{ .ClassName }}(QState* parent) : BasicState{parent} {
        {{ if .Wait }}
        setTimeout({{ .Timeout }});
        QObject::connect(this->timer(), &QTimer::timeout, this, [this]() {
			writeEndLogFailed("timeout");
			Q_EMIT this->ev_timeout();
		});
        {{ else }}
        setTimeout(-1);
        {{ end }}
    }

}
`

const FooImplCpp = `
#include <{{ .Path }}/{{ .FileName }}.h>
#include <logger.h>

namespace state {

	class {{ .ClassName }}Impl final : public {{ .ClassName }} {
	public:
		using {{ .ClassName }}::{{ .ClassName }};
		
		void perform() override {
			// todo
		}
		
		void clean() override {
			// todo
		}
	};

	{{ .ClassName }}* {{ .ClassName }}::create(QState* parent) {
		return new {{ .ClassName }}Impl{parent};
	}
}
`

const CMakeTPL = `
set(SRCS
{{ range . }}
autogen/{{ .Path }}/{{ .FileName }}.cpp
impl/{{ .Path }}/{{ .FileName }}_impl.cpp
{{ end }}
autogen/state_factory.cpp
)
    
add_library(states ${SRCS})
target_link_libraries(states Qt5::Core env_logger net_x51 testcase)
target_include_directories(states PUBLIC ${nlohmann-json_SOURCE_DIR} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/autogen)
target_compile_definitions(states PUBLIC JSON_MultipleHeaders=ON)
target_compile_options(states PRIVATE 
    -Wno-unknown-pragmas -Wno-unused-parameter -Wno-pedantic -Wno-missing-field-initializers 
    -Wno-unused-but-set-variable -Wno-sign-compare -Wno-misleading-indentation -Wno-unused-value
    -Wno-unused-variable -Wno-delete-non-virtual-dtor -Wno-reorder -Wno-ignored-qualifiers
    -Wno-unused-function -Wno-parentheses
)
`

const StateFactoryTPL = `
#include <testcase/idle.h>
{{ range . }}
#include "{{ .Path }}/{{ .FileName }}.h"
{{ end }}

QState* createStateByID(QString id, QState* parent) {
    if (id == "Idle") {
        return new Idle{parent};
    }

    {{ range . }}
    if (id == "{{ .Name }}") {
        return state::State{{ .Name }}::create(parent);
    }
    {{ end }}

    return nullptr;
}
`
