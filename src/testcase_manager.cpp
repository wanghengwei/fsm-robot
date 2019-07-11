#include "testcase_manager.h"
#include "testcase.h"
#include <pugixml.hpp>
#include "state_factory.h"
#include <memory>
#include "basic_state.h"
#include "states/idle.h"
#include <QtCore/QMap>
#include <QtCore/QDebug>
#include <QtCore/QFinalState>
#include <logger.h>
#include <fstream>
#include <robot/basic_robot.h>

// static std::shared_ptr<spdlog::logger> logger = spdlog::get("testcase_manager");

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
    void parseChildStates(pugi::xml_node const& root, QState* parent, QMap<QString, QAbstractState*>& states, QState* inner = nullptr) {
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
                // qDebug() << "duplicated id:", id;
                loggers::TESTCASE_PARSER().warn("duplicated state id: {}", id);
            }

            bool isParallel = nname == "parallel";

            // qDebug() << id << " before create state";
            QState* state = nullptr;
            if (id == "$_") {
                // qDebug() << "3333" << inner;
                Q_ASSERT_X(inner != nullptr, __PRETTY_FUNCTION__, "must pass inner argument!");
                state = inner;
                state->setParent(parent);
            } else {
                state = createState(type, parent, isParallel);
            }
            state->setObjectName(id);
            if (id == initName) {
                parent->setInitialState(state);
            }

            // 设置timeout，如果设置了属性timeout且 >= 0
            auto to = st.attribute("timeout").as_int(-1);
            auto p = qobject_cast<BasicState*>(state);
            if (to >= 0 && p != nullptr) {
                p->setTimeout(to);
            }
            // qDebug() << "new state " << state->objectName();

            // 递归处理子状态
            parseChildStates(st, state, states, inner);

            states[id] = state;
            // qDebug() << "add" << (isParallel ? "parallel" : "state") << id << "timeout:" << (p ? p->timeout() : 0);
            loggers::TESTCASE_PARSER().debug("add {}: id={}, timeout={}", (isParallel ? "parallel" : "state"), id, (p ? p->timeout() : 0));
        }

    }   

    void parseChildTransitions(pugi::xml_node const& root, QState* parent, QMap<QString, QAbstractState*>& states) {
        // for (auto &st : root.children()) {
        QString id = root.attribute("id").as_string();
        // state 是要处理trans的节点
        auto state = qobject_cast<QState*>(states[id]);
        if (!state) {
            // qDebug() << "WARNING: no state" << id;
            loggers::TESTCASE_PARSER().warn("no state {}", id);
            return;
        }
        for (auto &tr : root.children("transition")) {
            QString ev = tr.attribute("event").as_string();
            QString target = tr.attribute("target").as_string();
            QAbstractState* targetState = states[target];
            if (!targetState) {
                // qDebug() << "WARNING: no target" << id;
                loggers::TESTCASE_PARSER().warn("no target {}", id);
                continue;
            }

            // delay 表示这个trans会有一个延迟
            // 通过添加一个隐藏的idle节点来实现
            auto delay = tr.attribute("delay").as_int();

            if (ev.isEmpty()) {
                // 没有设置触发的event，那么就直接跳转
                if (delay == 0 && id == target) {
                    loggers::TESTCASE_PARSER().warn("cannot jump to self without delay: {}", id);
                    continue;
                }

                if (delay == 0) {
                    state->addTransition(targetState);
                } else {
                    // qDebug() << "add a temp idle in parent" << parent->objectName() << ", connect" << id << "and" << target;
                    loggers::TESTCASE_PARSER().debug("add a temp idle in parent {}, connect {} to {}", parent->objectName(), id, target);
                    Idle* idle = new Idle{parent};
                    idle->setTimeout(delay);
                    state->addTransition(idle);
                    idle->addTransition(idle, &Idle::ev_timeout, targetState);
                }
            } else {
                state->addTransition(state, qPrintable("2ev_" + ev + "()"), targetState);
            }
            // qDebug() << "add transition:" << state->objectName() << "--" << ev << "-->" << targetState->objectName() << "delay" << delay;
            loggers::TESTCASE_PARSER().debug("add transition: {} --{}--> {}, delay={}", state->objectName(), ev, targetState->objectName(), delay);
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
    }

    // 通过解析docroot，将root的内部状态图创建好
    void fillInner(QState* root, pugi::xml_node const& docroot, QState* inner = nullptr) {
        // qDebug() << "2222" << inner;
        // 这个map用来创建trans时通过id找到state
        QMap<QString, QAbstractState*> states;

        parseChildStates(docroot, root, states, inner);

        // 额外添加一个特殊状态叫finish
        QFinalState* finish = new QFinalState{root};
        finish->setObjectName("finish");
        QObject::connect(root, &QStateMachine::finished, []() {
            qDebug() << "finished";
        });
        states["finish"] = finish;

        for (auto& node : docroot.children()) {
            QString name = node.name();
            if (name != "state" && name != "parallel") {
                qDebug() << "WARNING: only state or parallel could at top level";
                continue;
            }
            parseChildTransitions(node, root, states);
        }
    }
}

TestCaseManager::TestCaseManager() {
    QObject::connect(&m_startTimer, &QTimer::timeout, this, &TestCaseManager::startSome);
}

void TestCaseManager::start() {
    for (auto& kv : m_testcases) {
        std::weak_ptr<TestCase> wp{kv.second};
        m_startQueue.emplace(std::move(wp));
    }

    if (m_speed < 10) {
        m_interval = 1000;
    } else if (m_speed < 60) {
        m_interval = 300;
    } else {
        m_interval = 100;
    }

    m_startTimer.start(m_interval);
}

void TestCaseManager::startSome() {
    int n = m_speed * m_interval / 1000;
    int k = 0;
    while (k < n && !m_startQueue.empty()) {
        ++k;
        auto p = m_startQueue.front().lock();
        m_startQueue.pop();
        if (!p) {
            continue;
        }
        p->start();
        // qDebug() << "start a testcase" << p->id();
    }
}

bool TestCaseManager::create(const std::string& id, const pugi::xml_document& doc) {
    // 创建一个用例对象
    std::shared_ptr<TestCase> testcase(new TestCase);
    // 初始化数据
    testcase->setObjectName("ROOT");
    testcase->setId(id);
    testcase->setData(m_userData);
    Q_ASSERT_X(m_connectionFactory, __PRETTY_FUNCTION__, "forgot set connectionFactory?");
    testcase->setConnectionFactory(m_connectionFactory);

    auto root = doc.child("testcase");
    auto pa = root.attribute("parent");
    if (pa.empty()) {
        fillInner(testcase.get(), root);
    } else {
        QState* s = new QState;
        fillInner(s, root);

        std::string ptp = pa.as_string();
        ptp = "../testcase-templates/" + ptp + ".xml";
        pugi::xml_document td;
        td.load_file(ptp.c_str());
        auto troot = td.child("testcase");
        // 解析模板
        // qDebug() << "1111111";
        fillInner(testcase.get(), troot, s);
    }

    // // 这个map用来创建trans时通过id找到state
    // QMap<QString, QAbstractState*> states;

    // parseChildStates(root, testcase.get(), states);

    // // 额外添加一个特殊状态叫finish
    // QFinalState* finish = new QFinalState{testcase.get()};
    // finish->setObjectName("finish");
    // QObject::connect(testcase.get(), &QStateMachine::finished, []() {
    //     qDebug() << "finished";
    // });
    // states["finish"] = finish;

    // for (auto& node : root.children()) {
    //     QString name = node.name();
    //     if (name != "state" && name != "parallel") {
    //         qDebug() << "WARNING: only state or parallel could at top level";
    //         continue;
    //     }
    //     parseChildTransitions(node, testcase.get(), states);
    // }

    m_testcases[id] = std::move(testcase);
    // qDebug() << "create testcase: account=" << id << ", casename=" << caseName;
    return true;
}

void TestCaseManager::createMany(const std::string& first, int count, const QString& caseName) {
    loggers::TESTCASE_MANAGER().info("create testcases: type=sequence, name={}, first={}, count={}", caseName, first, count);

    pugi::xml_document doc;

    auto tcpath = m_testcaseBaseDir.filePath(caseName);
    if (!tcpath.endsWith(".xml")) {
        tcpath += ".xml";
    }

    doc.load_file(qPrintable(tcpath));
    if (!doc) {
        return;
    }

    bool ok = true;
    auto lfirst = QString::fromStdString(first).toLongLong(&ok);
    if (!ok) {
        // 转换失败
        loggers::TESTCASE_MANAGER().error("first id is not number: {}", first);
        return;
    }

    for (int i = 0; i < count; i++) {
        std::string id = std::to_string(lfirst + i);
        this->create(id, doc);
    }

    loggers::TESTCASE_MANAGER().info("create testcases OK");
}

void TestCaseManager::loadUserData(QString const& d) {
    QDir dir;
    bool ok = dir.cd(d);
    if (!ok) {
        loggers::TESTCASE_MANAGER().error("cannot find userdata dir: {}", d);
        return;
    }

    // 目前感觉不需要有子目录

    // 最终合并成一个josn
    nlohmann::json merged = nlohmann::json::object();

    auto dfiles = dir.entryList(QStringList{} << "*.json");
    for (auto& df : dfiles) {
        auto p = dir.filePath(df);
        std::ifstream ins{p.toStdString()};
        if (!ins) {
            loggers::TESTCASE_MANAGER().error("cannot read userdata file: {}", df);
            return;
        }

        nlohmann::json j;
        try {
            ins >> j;
        } catch (std::exception& ex) {
            loggers::TESTCASE_MANAGER().error("cannot parse json file {}: what={}", df, ex.what());
            return;
        }

        merged.merge_patch(j);
    }

    loggers::TESTCASE_MANAGER().trace("merged userdata: {}", merged.dump());

    this->m_userData = std::move(merged);
}