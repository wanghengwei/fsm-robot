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
	projectRoot = flag.String("R", ".", "project root dir")
)

type State struct {
	Name        string
	ClassName   string
	FileName    string
	Description string
	Timeout     int
	Wait        bool
	PrintLog    bool `yaml:"printLog"`
	Signals     []string
}

func main() {
	// flag.Parse()

	statedef := flag.String("s", "", "yaml of states")
	flag.Parse()

	fmt.Println(*statedef)

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

		p := filepath.Join(*projectRoot, "states", "impl", fmt.Sprintf("%s_impl.cpp", state.FileName))
		_, err = os.Stat(p)
		if os.IsNotExist(err) {
			f, err := os.Create(p)
			if err != nil {
				panic(err)
			}
			err = fooImplCpp.Execute(f, state)
			// err = writeStateFile(fooImplCpp, &state, false)
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
	f, err = os.Create(filepath.Join(*projectRoot, "states", "autogen", "CMakeLists.txt"))
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
	f, err = os.Create(filepath.Join(*projectRoot, "state_factory.cpp"))
	if err != nil {
		panic(err)
	}
	err = tpl.Execute(f, &states)
	if err != nil {
		panic(err)
	}
}

func writeStateFile(tpl *template.Template, state *State, overwrite bool) error {
	p := filepath.Join(*projectRoot, "states", "autogen", fmt.Sprintf("%s%s", state.FileName, tpl.Name()))
	_, err := os.Stat(p)
	if !overwrite && !os.IsNotExist(err) {
		return nil
	}

	f, err := os.OpenFile(p, os.O_TRUNC|os.O_CREATE|os.O_WRONLY, 0644)
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
#include <basic_state.h>

namespace state {

// 状态描述：{{ .Description }}
class {{ .ClassName }} final : public BasicState {
    Q_OBJECT
public:
    explicit {{ .ClassName }}(QState* parent = nullptr);
private:
    void perform(std::map<std::string, std::string>& info) override;
	bool printLog() const override;
signals:
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
        QObject::connect(this->timer(), &QTimer::timeout, this, &BasicState::ev_timeout);
        {{ else }}
        setTimeout(-1);
        {{ end }}
    }

	bool {{ .ClassName }}::printLog() const { return {{ .PrintLog }}; }
}
`

const FooImplCpp = `
#include "{{ .FileName }}.h"
#include <stdexcept>
#include <logger.h>

namespace state {

    void {{ .ClassName }}::perform(std::map<std::string, std::string>& info) {
        // todo
        throw std::runtime_error{"todo"};
    }

}
`

const CMakeTPL = `
set(SRCS
{{ range . }}
{{ .FileName }}.cpp
../impl/{{ .FileName }}_impl.cpp
{{ end }}
)
    
add_library(states_gen ${SRCS})
target_link_libraries(states_gen Qt5::Core env_logger)
target_include_directories(states_gen PUBLIC ${nlohmann-json_SOURCE_DIR})
target_compile_definitions(states_gen PUBLIC JSON_MultipleHeaders=ON)
`

const StateFactoryTPL = `
#include "state_factory.h"
#include "states/idle.h"
{{ range . }}
#include "states/autogen/{{ .FileName }}.h"
{{ end }}

QState* createStateByID(QString id, QState* parent) {
    if (id == "Idle") {
        return new Idle{parent};
    }

    {{ range . }}
    if (id == "{{ .Name }}") {
        return new state::State{{ .Name }}{parent};
    }
    {{ end }}

    return nullptr;
}
`
