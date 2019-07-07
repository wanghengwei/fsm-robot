#include "testcase.h"
#include <nlohmann/json.hpp>

namespace impl {
    void setData(const nlohmann::json& data, std::map<QString, std::any>& store) {
        store.clear();
        for (auto& p : data.items()) {
            QString k = QString::fromStdString(p.key());
            auto v = p.value();
            switch (v.type()) {
                case nlohmann::json::value_t::number_integer: {
                    store[k] = v.get<int64_t>();
                    break;
                }
                case nlohmann::json::value_t::number_unsigned: {
                    store[k] = v.get<int64_t>();
                    break;
                }
                case nlohmann::json::value_t::number_float: {
                    store[k] = v.get<double>();
                    break;
                }
                case nlohmann::json::value_t::string: {
                    store[k] = v.get<std::string>();
                    break;
                }
                case nlohmann::json::value_t::boolean: {
                    store[k] = v.get<bool>();
                    break;
                }
                case nlohmann::json::value_t::null: {
                    // m_data[k] = v.get<uint64_t>();
                    break;
                }
                default: {
                    // array 和 object 直接保存
                    store[k] = v;
                    break;
                }
            }
        }
    }
}

void TestCase::setData(const nlohmann::json& data) {
    impl::setData(data, m_data);
}