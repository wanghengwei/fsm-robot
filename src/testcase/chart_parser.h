#pragma once
#include <QtCore/QState>
#include <pugixml.hpp>
#include "xdoc_manager.h"
#include <memory>
#include <QtCore/QMap>
#include <QtCore/QFinalState>
#include <QtCore/QStateMachine>
#include "testcase.h"

struct ChartParser {
    QString chartId; // chart的id，基本上就是其文件名或路径
    std::shared_ptr<XDocManager> xdocManager;
    QMap<QString, QState*> idTable; // tmp
    QState* child = nullptr;
    QFinalState* finish = nullptr;
    QState* parsedRootState = nullptr; // output

    bool parse(const pugi::xml_document& doc);
    // bool parseRootState(const pugi::xml_document& doc);
    bool parseChildStates(pugi::xml_node const& rootNode, QState* root);
    bool parseInitialStates(pugi::xml_node const& rootNode, QState* root);
    bool parseTransitions(pugi::xml_node const& rootNode, QState* root);
    bool setFinishTransitions(pugi::xml_node const& rootNode, QState* root, QFinalState* finish);
    bool parseTransitionToParent(pugi::xml_node const& rootNode, QState* root, QMap<QString, QState*> const& m);
};

/** 
 * @brief 解析用例xml
*/
TestCase* parseChart(const QString& id, std::shared_ptr<XDocManager> dm);