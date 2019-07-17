#pragma once
#include <pugixml.hpp>
#include <memory>
#include <QtCore/QString>
#include <QtCore/QMap>

class XDocManager {
public:
    std::shared_ptr<pugi::xml_document> getDoc(const QString& cid);
    std::shared_ptr<pugi::xml_document> getTemplate(const QString& tid);
private:
    QMap<QString, std::shared_ptr<pugi::xml_document>> m_doccache;
    QMap<QString, std::shared_ptr<pugi::xml_document>> m_tplcache;
};