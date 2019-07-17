#include "chart_parser.h"
#include "state_factory.h"
#include <logger.h>
#include <QtCore/QDebug>
#include "idle.h"
#include "testcase.h"
#include <QtCore/QFinalState>
#include <QtCore/QScopedPointer>

bool ChartParser::parse(const pugi::xml_document& doc) {
    if (!doc) {
        loggers::TESTCASE_PARSER().error("cannot load chart: {}", chartId);
        return false;
    }

    auto rootNode = doc.child("state");
    if (!rootNode) {
        loggers::TESTCASE_PARSER().error("root node is not state: {}", chartId);
        return false;
    }

    QState* root = new QState;

    // 将所有state初始化
    bool ok = parseChildStates(rootNode, root);
    if (!ok) {
        delete root;
        return false;
    }

    // 处理initial state
    ok = parseInitialStates(rootNode, root);
    if (!ok) {
        delete root;
        return false;
    }

    // 处理trans
    ok = parseTransitions(rootNode, root);
    if (!ok) {
        delete root;
        return false;
    }

    parsedRootState = root;

    // 如果有指定parent，那么加载一下parent模板
    QString pname = rootNode.attribute("parent").as_string();
    if (!pname.isEmpty()) {
        auto pdoc = xdocManager->getTemplate(pname);
        if (!pdoc) {
            loggers::TESTCASE_PARSER().error("no parent template: {}", pname);
            delete root;
            parsedRootState = nullptr;
            return false;
        }

        loggers::TESTCASE_PARSER().info("found parent, parsing parent...");
        ChartParser pparser;
        pparser.xdocManager = xdocManager;
        pparser.chartId = pname;
        pparser.child = root;
        pparser.finish = this->finish;
        bool ok = pparser.parse(*pdoc);
        if (!ok) {
            loggers::TESTCASE_PARSER().error("parse parent failed: {}", pname);
            delete root;
            parsedRootState = nullptr;
            return false;
        }

        // 处理一下child里面target等于parent.xxx的trans
        parseTransitionToParent(rootNode, root, pparser.idTable);

        root = pparser.parsedRootState;
        parsedRootState = root;
    }

    return true;
}

bool ChartParser::parseTransitionToParent(pugi::xml_node const& rootNode, QState* root, QMap<QString, QState*> const& m) {
    QString id = rootNode.attribute("id").as_string();
    // QState* root = id.isEmpty() ? parsedRootState : idTable.value(id);
    
    Q_ASSERT(root != nullptr);

    for (auto& cn : rootNode.children("transition")) {
        QString targetId = cn.attribute("target").as_string();
        QString ev = cn.attribute("event").as_string();
        int delay = cn.attribute("delay").as_int();

        if (!targetId.startsWith("parent.")) {
            continue;
        }

        targetId = targetId.mid(7);
        if (targetId.isEmpty()) {
            loggers::TESTCASE_PARSER().error("parentId is empty: stateid={}", id);
            return false;
        }

        QState* targetState = m.value(targetId);
        if (targetState == nullptr) {
            loggers::TESTCASE_PARSER().error("target parent state is null: target={}", targetId);
            return false;
        }

        if (ev.isEmpty()) {
            // 没有设置触发的event，那么就直接跳转
            if (delay <= 0) {
                root->addTransition(targetState);
            } else {
                QState* parent = root->parentState();
                if (parent == nullptr) {
                    loggers::TESTCASE_PARSER().error("cannot add trans at root node");
                    return false;
                }
                QString pn = parent->objectName();
                loggers::TESTCASE_PARSER().info("add a temp idle in parent {}, connect {} to parent.{}", pn, id, targetId);
                Idle* idle = new Idle{root->parentState()};
                idle->setTimeout(delay);
                root->addTransition(idle);
                idle->addTransition(idle, &Idle::ev_timeout, targetState);
            }
        } else {
            root->addTransition(root, qPrintable("2ev_" + ev + "()"), targetState);
        }
        // qDebug() << "add transition:" << state->objectName() << "--" << ev << "-->" << targetState->objectName() << "delay" << delay;
        loggers::TESTCASE_PARSER().info("add transition: {} --{}--> parent.{}, delay={}", root->objectName(), ev, targetState->objectName(), delay);
    }

    for (auto& cn : rootNode.children()) {
        QString name = cn.name();
        if (name != "state" && name != "parallel") {
            continue;
        }

        QString cid = cn.attribute("id").as_string();
        QState* cs = idTable.value(cid);
        Q_ASSERT(cs != nullptr);

        bool ok = parseTransitionToParent(cn, cs, m);
        if (!ok) {
            return false;
        }
    }

    return true;
}

bool ChartParser::parseChildStates(pugi::xml_node const& rootNode, QState* root) {
    // 有两种child要处理：state和parallel
    for (auto const& n : rootNode.children()) {
        QString nname = n.name();
        if (nname != "state" && nname != "parallel") {
            continue;
        }

        QString id = n.attribute("id").as_string();
        if (id.isEmpty()) {
            loggers::TESTCASE_PARSER().error("id is required: {}", nname);
            return false;
        }

        QState* s = nullptr;

        if (id == "$_") {
            // 如果id是$_，说明要嵌入子状态
            Q_ASSERT(this->child != nullptr);
            loggers::TESTCASE_PARSER().info("using child chart here");
            s = this->child;
            s->setParent(root);
            s->setObjectName("$_");
            idTable[id] = s;
        } else {
            QString st = n.attribute("type").as_string();
            if (st.isEmpty()) {
                st = id;
            }

            // 不允许重复的id
            auto it = idTable.find(id);
            if (it != idTable.end()) {
                loggers::TESTCASE_PARSER().error("duplicated id: {}", id);
                return false;
            }

            s = createStateByID(st, root);
            if (s == nullptr) {
                loggers::TESTCASE_PARSER().debug("cannot find custom state {}, using QState", id);
                s = new QState{root};
            } else {
                // 设置timeout，如果设置了属性timeout且 >= 0
                auto to = n.attribute("timeout").as_int(-1);
                auto p = qobject_cast<BasicState*>(s);
                if (to >= 0 && p != nullptr) {
                    p->setTimeout(to);
                }
            }
        
            s->setObjectName(id);
            if (nname == "parallel") {
                s->setChildMode(QState::ParallelStates);
            }

            idTable[id] = s;
            loggers::TESTCASE_PARSER().info("add state: {}", id);

            bool ok = parseChildStates(n, s);
            if (!ok) {
                return false;
            }
        }
    }

    return true;
}

bool ChartParser::parseInitialStates(pugi::xml_node const& rootNode, QState* root) {
    QString name = rootNode.name();
    QString id = rootNode.attribute("id").as_string();
    if (id == "$_") {
        return true;
    }

    // 如果没有子节点，那么就不需要init
    auto t = root->findChildren<QState*>();
    if (t.empty()) {
        return true;
    }

    

    if (name == "state") {
        QString init = rootNode.attribute("initial").as_string();
        if (init.isEmpty()) {
            loggers::TESTCASE_PARSER().error("attr initial is empty: {}", root->objectName());
        }
        auto initnode = root->findChild<QState*>(init, Qt::FindDirectChildrenOnly);
        if (!initnode) {
            loggers::TESTCASE_PARSER().error("cannot find init state for {}: {}", root->objectName(), init);
            return false;
        }

        root->setInitialState(initnode);
        loggers::TESTCASE_PARSER().info("set init state for {}: {}", root->objectName(), init);
    }

    // 有两种child要处理：state和parallel
    for (auto const& n : rootNode.children()) {
        QString nname = n.name();
        if (nname != "state" && nname != "parallel") {
            continue;
        }

        QString id = n.attribute("id").as_string();
        QState* s = idTable.value(id);
        Q_ASSERT(s != nullptr);

        bool ok = parseInitialStates(n, s);
        if (!ok) {
            return false;
        }
    }

    return true;
}


bool ChartParser::parseTransitions(pugi::xml_node const& rootNode, QState* root) {
    QString name = rootNode.name();
    QString rootid = root->objectName();
    if (rootid == "$_") {
        return true;
    }

    for (auto& tn : rootNode.children("transition")) {
        QString ev = tn.attribute("event").as_string();
        QString target = tn.attribute("target").as_string();
        // delay 表示这个trans会有一个延迟
        // 通过添加一个隐藏的idle节点来实现
        int delay = tn.attribute("delay").as_int();

        QAbstractState* targetState = nullptr;

        if (target == "finish") {
            // finish特殊对待，需要指向一个全局唯一的final state
            Q_ASSERT(this->finish != nullptr);
            targetState = this->finish;    
        } else if (target.startsWith("parent.")) {
            // 这种指向父节点状态的trans也要之后再处理
            continue;
        } else {
            targetState = idTable.value(target);
        }

        if (targetState == nullptr) {
            loggers::TESTCASE_PARSER().error("no target {}", target);
            return false;
        }

        if (ev.isEmpty()) {
            // 没有设置触发的event，那么就直接跳转
            if (delay <= 0 && rootid == target) {
                loggers::TESTCASE_PARSER().error("cannot jump to self without delay: {}", rootid);
                continue;
            }

            if (delay <= 0) {
                root->addTransition(targetState);
            } else {
                QState* parent = root->parentState();
                if (parent == nullptr) {
                    loggers::TESTCASE_PARSER().error("cannot add trans at root node");
                    return false;
                }
                QString pn = parent ? parent->objectName() : "";
                loggers::TESTCASE_PARSER().info("add a temp idle in parent {}, connect {} to {}", pn, rootid, target);
                Idle* idle = new Idle{root->parentState()};
                idle->setTimeout(delay);
                root->addTransition(idle);
                idle->addTransition(idle, &Idle::ev_timeout, targetState);
            }
        } else {
            root->addTransition(root, qPrintable("2ev_" + ev + "()"), targetState);
        }
        // qDebug() << "add transition:" << state->objectName() << "--" << ev << "-->" << targetState->objectName() << "delay" << delay;
        loggers::TESTCASE_PARSER().info("add transition: {} --{}--> {}, delay={}", root->objectName(), ev, targetState->objectName(), delay);
    }

    // 有两种child要处理：state和parallel
    for (auto const& n : rootNode.children()) {
        QString nname = n.name();
        if (nname != "state" && nname != "parallel") {
            continue;
        }

        QString id = n.attribute("id").as_string();
        QState* s = idTable.value(id);
        Q_ASSERT(s != nullptr);

        bool ok = parseTransitions(n, s);
        if (!ok) {
            return false;
        }
    }

    return true;
}



bool ChartParser::setFinishTransitions(pugi::xml_node const& rootNode, QState* root, QFinalState* finish) {
    QString name = rootNode.name();
    QString rootid = root->objectName();
    for (auto& tn : rootNode.children("transition")) {
        QString ev = tn.attribute("event").as_string();
        QString target = tn.attribute("target").as_string();
        if (target != "finish") {
            continue;
        }

        // delay 表示这个trans会有一个延迟
        // 通过添加一个隐藏的idle节点来实现
        int delay = tn.attribute("delay").as_int();

        if (ev.isEmpty()) {
            // 没有设置触发的event，那么就直接跳转
            if (delay <= 0 && rootid == target) {
                loggers::TESTCASE_PARSER().error("cannot jump to self without delay: {}", rootid);
                continue;
            }

            if (delay <= 0) {
                root->addTransition(finish);
            } else {
                QState* parent = root->parentState();
                if (parent == nullptr) {
                    loggers::TESTCASE_PARSER().error("cannot add trans at root node");
                    return false;
                }
                QString pn = parent ? parent->objectName() : "";
                loggers::TESTCASE_PARSER().info("add a temp idle in parent {}, connect {} to {}", pn, rootid, target);
                Idle* idle = new Idle{root->parentState()};
                idle->setTimeout(delay);
                root->addTransition(idle);
                idle->addTransition(idle, &Idle::ev_timeout, finish);
            }
        } else {
            root->addTransition(root, qPrintable("2ev_" + ev + "()"), finish);
        }
        // qDebug() << "add transition:" << state->objectName() << "--" << ev << "-->" << targetState->objectName() << "delay" << delay;
        loggers::TESTCASE_PARSER().info("add transition: {} --{}--> {}, delay={}", root->objectName(), ev, "finish", delay);
    }

    // 有两种child要处理：state和parallel
    for (auto const& n : rootNode.children()) {
        QString nname = n.name();
        if (nname != "state" && nname != "parallel") {
            continue;
        }

        QString id = n.attribute("id").as_string();
        QState* s = idTable.value(id);
        Q_ASSERT(s != nullptr);

        bool ok = setFinishTransitions(n, s, finish);
        if (!ok) {
            return false;
        }
    }

    return true;
}

TestCase* parseChart(const QString& id, std::shared_ptr<XDocManager> dm) {
    auto doc = dm->getDoc(id);
    if (!doc) {
        loggers::TESTCASE_PARSER().error("cannot load state chart: {}", id);
        return nullptr;
    }

    QFinalState* finish = new QFinalState;
    finish->setObjectName("finish");

    ChartParser parser;
    parser.chartId = id;
    parser.finish = finish;
    parser.xdocManager = dm;

    bool ok = parser.parse(*doc);
    if (!ok) {
        delete finish;
        return nullptr;
    }

    Q_ASSERT(parser.parsedRootState != nullptr);

    TestCase* m = new TestCase;
    m->addState(parser.parsedRootState);
    m->setInitialState(parser.parsedRootState);
    m->addState(finish);

    return m;
}
