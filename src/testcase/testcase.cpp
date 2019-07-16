#include "testcase.h"
#include <nlohmann/json.hpp>
#include <net_base/basic_robot.h>

namespace impl {
    void setData(const nlohmann::json& data, std::map<std::string, std::any>& store) {
        store.clear();
        for (auto& p : data.items()) {
            auto k = p.key();
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

TestCase::TestCase(QObject* parent) : QStateMachine{parent} {
    m_robot = new BasicRobot{};
}

TestCase::~TestCase() {
    delete m_robot;
}

void TestCase::setConnectionFactory(const std::shared_ptr<BasicConnectionFactory>& p) {
    this->robot().setConnectionFactory(p);
}

void TestCase::setData(const nlohmann::json& data) {
    impl::setData(data, m_data);
}

BasicRobot& TestCase::robot() {
    Q_ASSERT_X(m_robot != nullptr, "robot()", "forgot init robot?");
    return *m_robot;
}

void TestCase::insertOrUpdateData(const std::string& key, const std::any& value) {
    m_data[key] = value;
}

bool TestCase::insertData(const std::string& key, const std::any& value) {
    auto r = m_data.insert(std::make_pair(key, value));
    return r.second;
}