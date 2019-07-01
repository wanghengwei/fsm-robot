#include "testcase_manager.h"
#include "testcase.h"
#include <pugixml.hpp>
#include "state_factory.h"
#include <memory>
#include "base_state.h"
#include "states/idle.h"
#include <QtCore/QMap>
#include <QtCore/QDebug>
#include <QtCore/QFinalState>


namespace {
    // 根据parent来创建并加入state。如果parent是state machine，就调addstate，否则就作为子state
    QState* createState(QString type, QState* parent, bool isParallel = false) {
        QState* state = nullptr;
        if (auto m = qobject_cast<QStateMachine*>(parent)) {
            state = createStateByID(type);
            if (!state) {
                // 如果state是空表示并不是自动生成的，那么用个通用的qstate
                state = new QState;
            }
            m->addState(state);
        } else {
            state = createStateByID(type, parent);
            if (!state) {
                state = new QState{parent};
            }
        }
        // state->setObjectName(id);
        if (isParallel) {
            state->setChildMode(QState::ParallelStates);
        }
        return state;
    }

    // 将全部的state找出来
    void parseChildStates(pugi::xml_node const& root, QState* parent, QMap<QString, QAbstractState*>& states) {
        QString rootName = root.name();
        QString initName = root.attribute("initial").as_string();
        // 如果没有指定初始状态，那么就不要处理其内部状态了
        // 有个例外，就是parallel
        if (initName.isEmpty() && rootName != "parallel") {
            return;
        }

        // 临时保存一个map，用于创建transition时使用
        // QMap<QString, QAbstractState*> states;

        // 首先添加全部的状态，包括parallel
        for (auto& st : root.children()) {
            QString nname = st.name();
            if (nname == "transition") {
                // qDebug() << "skip trans";
                continue;
            }

            QString id = st.attribute("id").as_string();
            // 为了方便，type可以不写。不写时就等于id。但有时候会有多个同样类型的状态，这时候就要用id区分了
            QString type = st.attribute("type").as_string();
            if (type.isEmpty()) {
                type = id;
            }

            // id不能重复
            if (states.find(id) != states.end()) {
                qDebug() << "duplicated id:", id;
            }

            bool isParallel = nname == "parallel";

            // qDebug() << id << " before create state";
            auto state = createState(type, parent, isParallel);
            state->setObjectName(id);
            if (id == initName) {
                parent->setInitialState(state);
            }

            // 设置timeout，如果设置了属性timeout且 >= 0
            auto to = st.attribute("timeout").as_int(-1);
            if (to >= 0) {
                if (auto p = qobject_cast<BaseState*>(state)) {
                    p->setTimeout(to);
                }
            }
            // qDebug() << "new state " << state->objectName();

            // 递归处理子状态
            parseChildStates(st, state, states);

            states[id] = state;
            qDebug() << "add" << (isParallel ? "parallel" : "state") << id;
        }

    }   

    void parseChildTransitions(pugi::xml_node const& root, QState* parent, QMap<QString, QAbstractState*>& states) {
        // for (auto &st : root.children()) {
        QString id = root.attribute("id").as_string();
        // state 是要处理trans的节点
        auto state = qobject_cast<QState*>(states[id]);
        if (!state) {
            qDebug() << "WARNING: no state" << id;
            return;
        }
        for (auto &tr : root.children("transition")) {
            QString ev = tr.attribute("event").as_string();
            QString target = tr.attribute("target").as_string();
            QAbstractState* targetState = states[target];
            if (!targetState) {
                qDebug() << "WARNING: no target" << id;
                continue;
            }

            // delay 表示这个trans会有一个延迟
            // 通过添加一个隐藏的idle节点来实现
            auto delay = tr.attribute("delay").as_int();

            if (ev.isEmpty()) {
                // 没有设置触发的event，那么就直接跳转
                if (delay == 0 && id == target) {
                    qDebug() << "WARNING: cannot jump to self without delay:" << id;
                    continue;
                }

                if (delay == 0) {
                    state->addTransition(targetState);
                } else {
                    qDebug() << "add a temp idle in parent" << parent->objectName() << ", connect" << id << "and" << target;
                    Idle* idle = new Idle{parent};
                    idle->setTimeout(delay);
                    state->addTransition(idle);
                    idle->addTransition(idle, &Idle::ev_timeout, targetState);
                }
            } else {
                state->addTransition(state, qPrintable("2ev_" + ev + "()"), targetState);
            }
            qDebug() << "add transition:" << state->objectName() << "--" << ev << "-->" << targetState->objectName() << "delay" << delay;
        }

        // 把子状态也递归处理了
        for (auto& node : root.children()) {
            QString name = node.name();
            if (name == "transition") {
                continue;
            }

            // QString id = node.attribute("id").as_string();

            // QState* substate = states[id];

            parseChildTransitions(node, state, states);
        }
        // }
    }
}

void TestCaseManager::start() {
    for (auto& kv : m_testcases) {
        kv.second->start();
    }
}

void TestCaseManager::create(QString account, QString caseName) {
    pugi::xml_document doc;
    doc.load_file(qPrintable(caseName));

    std::unique_ptr<TestCase> testcase(new TestCase);

    QMap<QString, QAbstractState*> states;

    auto root = doc.child("testcase");

    parseChildStates(root, testcase.get(), states);
    QFinalState* finish = new QFinalState{testcase.get()};
    finish->setObjectName("finish");
    states["finish"] = finish;

    for (auto& node : root.children()) {
        QString name = node.name();
        if (name != "state" && name != "parallel") {
            qDebug() << "WARNING: only state or parallel could at top level";
            continue;
        }
        parseChildTransitions(node, testcase.get(), states);
    }

    m_testcases[account] = std::move(testcase);
    qDebug() << "create testcase: account=" << account << ", casename=" << caseName;
}
