#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <QtCore/QDebug>
#include <any>
#include <map>
#include "testcase.h"

TEST(testcase_test, test_json) {
    nlohmann::json j {
        {"a", "b"},
        {"c", "d"},
        {"e", 147483648},
    };

    for (auto& p : j.items()) {
        qDebug() << p.key().c_str() << p.value().dump().c_str();        
    }

    qDebug() << int(j["e"].type());

    qDebug() << j["e"].get<int64_t>();

    qDebug() << j["a"].get<std::string>().c_str();
}

namespace impl {
    extern void setData(const nlohmann::json& data, std::map<std::string, std::any>& store);
}

TEST(testcase_test, test_set_data) {
    std::map<std::string, std::any> store;

    impl::setData({}, store);
    ASSERT_TRUE(store.empty());

    impl::setData({
        {"a", 1}
    }, store);

    ASSERT_EQ(1, std::any_cast<int64_t>(store["a"]));

    impl::setData({
        {"a", 91234567890}
    }, store);

    ASSERT_EQ(91234567890, std::any_cast<int64_t>(store["a"]));

    impl::setData({
        {"a", 91234567890u}
    }, store);

    ASSERT_EQ(91234567890, std::any_cast<int64_t>(store["a"]));

    impl::setData({
        {"a", "b"}
    }, store);

    ASSERT_EQ("b", std::any_cast<std::string>(store["a"]));

    impl::setData({
        {"a", true}
    }, store);

    ASSERT_EQ(true, std::any_cast<bool>(store["a"]));

    impl::setData({
        {"a", {1, 2, 3}}
    }, store);

    ASSERT_EQ(nlohmann::json::array({1, 2, 3}), std::any_cast<nlohmann::json>(store["a"]));

    impl::setData({
        {"a", {
            {"b", 1}
        }}
    }, store);

    ASSERT_EQ((nlohmann::json{{"b", 1}}), std::any_cast<nlohmann::json>(store["a"]));
}

// TEST(testcase_test, test_get_any) {
//     {
//         std::any a = int64_t(1);
//         int v = 0;
//         bool ok = impl::GetValueFromAny<int>{}(a, v);
//         ASSERT_TRUE(ok);
//         ASSERT_EQ(1, v);
//     }

//     {
//         std::any a = uint64_t(1);
//         uint32_t v = 0;
//         bool ok = impl::GetValueFromAny<uint32_t>{}(a, v);
//         ASSERT_TRUE(ok);
//         ASSERT_EQ(1u, v);
//     }

//     {
//         std::any a = true;
//         bool v = false;
//         bool ok = impl::GetValueFromAny<bool>{}(a, v);
//         ASSERT_TRUE(ok);
//         ASSERT_EQ(true, v);
//     }

//     {
//         std::any a = double{1.23};
//         double v;
//         bool ok = impl::GetValueFromAny<double>{}(a, v);
//         ASSERT_TRUE(ok);
//         ASSERT_EQ(1.23, v);
//     }
// }

// TEST(testcase_test, test_testcase_set) {
//     TestCase t;
//     std::string s = R"#(
//     {
//         "a": 1,
//         "b": 91234567890,
//         "c": "C",
//         "d": true,
//         "e": {"E": 1}
//     }
//     )#";
//     nlohmann::json j = nlohmann::json::parse(s);
//     qDebug() << int(j["a"].type());
//     qDebug() << j["e"].dump().c_str();
//     t.setData(j);

//     {
//         int v;
//         bool ok = t.getData("a", v);
//         ASSERT_TRUE(ok);
//         ASSERT_EQ(1, v);
//     }

//     {
//         long long v;
//         bool ok = t.getData("b", v);
//         ASSERT_TRUE(ok);
//         ASSERT_EQ(91234567890, v);
//     }

//     {
//         unsigned long long v;
//         bool ok = t.getData("b", v);
//         ASSERT_FALSE(ok);
//         // ASSERT_EQ(91234567890, v);
//     }

//     {
//         std::string v;
//         bool ok = t.getData("c", v);
//         ASSERT_TRUE(ok);
//         ASSERT_EQ("C", v);
//     }

//     {
//         bool v;
//         bool ok = t.getData("d", v);
//         ASSERT_TRUE(ok);
//         ASSERT_EQ(true, v);
//     }

//     {
//         nlohmann::json v;
//         bool ok = t.getData("e", v);
//         ASSERT_TRUE(ok);
//         qDebug() << v.dump().c_str();
//         ASSERT_EQ(1, v["E"]);
//     }
// }
