#pragma once
#include <pugixml.hpp>
#include <memory>
#include <QtCore/QString>

class XDocManager {
public:
    std::shared_ptr<pugi::xml_document> getDoc(const QString& cid);
    std::shared_ptr<pugi::xml_document> getTemplate(const QString& tid);
};