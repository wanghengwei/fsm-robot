#include "xdoc_manager.h"
#include <QtCore/QDir>
#include <logger.h>
#include <QtCore/QDebug>

std::shared_ptr<pugi::xml_document> XDocManager::getDoc(const QString& cid) {
    QDir base{"../testcases"};
    auto xp = base.filePath(cid + ".xml");
    std::shared_ptr<pugi::xml_document> doc{new pugi::xml_document};
    auto r = doc->load_file(qPrintable(xp), pugi::parse_default, pugi::encoding_utf8);
    if (!r) {
        loggers::TESTCASE_PARSER().error("cannot load xml: {}", xp);
        return nullptr;
    }

    // for (auto& n : doc.child("state").children()) {
    //     qDebug() << n.name();
    // }

    return doc;
}

std::shared_ptr<pugi::xml_document> XDocManager::getTemplate(const QString& cid) {
    QDir base{"../testcase-templates"};
    auto xp = base.filePath(cid + ".xml");
    std::shared_ptr<pugi::xml_document> doc{new pugi::xml_document};
    auto r = doc->load_file(qPrintable(xp), pugi::parse_default, pugi::encoding_utf8);
    if (!r) {
        loggers::TESTCASE_PARSER().error("cannot load xml: {}", xp);
        return nullptr;
    }

    // for (auto& n : doc.child("state").children()) {
    //     qDebug() << n.name();
    // }

    return doc;
}
