#pragma once
#include <QtCore/QStateMachine>
#include <QtCore/QDebug>
#include <any>
#include <type_traits>
#include <nlohmann/json.hpp>
#include <logger.h>

class BasicRobot;
class BasicConnectionFactory;

class TestCase : public QStateMachine {
    Q_OBJECT

public:
    explicit TestCase(QObject* parent = nullptr);

    ~TestCase();

    void setConnectionFactory(const std::shared_ptr<BasicConnectionFactory>& p);

    const std::string& id() const { return m_userId; }
    
    /** 
     * @brief 设置此用例对象使用的玩家账号id
    */
    void setId(const std::string& id) { m_userId = id; }

    /** 
     * @brief 设置初始用户数据，用户一般不调用这个
     * 
     * 无法保存超过int64_t范围的整数
    */
    void setConfig(const nlohmann::json& cfg);

    template<typename T>
    bool __attribute__ ((warn_unused_result)) getConfig(const std::string& jpath, T& value);

    /** 
     * @brief 是否已存在某个key的玩家数据
    */
    bool hasData(const std::string& key) const {
        return m_data.find(key) != m_data.end();
    }

    /** 
     * @brief 根据key获得一个用户数据
     * 
     * 如果key不存在，返回false
     * 
    */
    template<typename T>
    bool __attribute__ ((warn_unused_result)) getData(const std::string& key, T& value);

    /** 
     * @brief 插入或更新用户数据
    */
    void insertOrUpdateData(const std::string& key, const std::any& value);

    /**
     * @brief 插入数据
     * 
     * @return 成功插入返回true
     * 
     * 如果key已存在，则不插入
     * 
    */
    bool insertData(const std::string& key, const std::any& value);

    BasicRobot& robot();

private:
    std::string m_userId;

    // 保存k-v结构的用户数据
    std::map<std::string, std::any> m_data;
    nlohmann::json m_config;

    BasicRobot* m_robot = nullptr;
};

// namespace impl {
//     template<typename T, typename Enabled = void>
//     struct GetValueFromAny {
//         bool operator()(const std::any& a, T& v) {
//             try {
//                 v = std::any_cast<T>(a);
//                 return true;
//             } catch (std::bad_any_cast const& ex) {
//                 return false;
//             }
//         }
//     };

//     // 所有的有符号整数
//     template<typename T>
//     struct GetValueFromAny<T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>> {
//         bool operator()(const std::any& a, T& v) {
//             try {
//                 // qDebug() << "111";
//                 v = T(std::any_cast<int64_t>(a));
//                 return true;
//             } catch (std::bad_any_cast const& ex) {
//                 return false;
//             }
//         }
//     };

//     template<typename T>
//     struct GetValueFromAny<T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>> {
//         bool operator()(const std::any& a, T& v) {
//             try {
//                 // qDebug() << "111";
//                 v = T(std::any_cast<uint64_t>(a));
//                 return true;
//             } catch (std::bad_any_cast const& ex) {
//                 return false;
//             }
//         }
//     };

//     template<>
//     struct GetValueFromAny<bool> {
//         bool operator()(const std::any& a, bool& v) {
//             try {
//                 v = std::any_cast<bool>(a);
//                 return true;
//             } catch (std::bad_any_cast const& ex) {
//                 return false;
//             }
//         }
//     };
// }


template<typename T>
bool __attribute__ ((warn_unused_result)) TestCase::getData(const std::string& key, T& value) {
    auto it = m_data.find(key);
    if (it == m_data.end()) {
        return false;
    }

    // return impl::GetValueFromAny<T>{}(it->second, value);
    try {
        value = std::any_cast<T>(it->second);
        return true;
    } catch (std::bad_any_cast const& ex) {
        return false;
    }
}

template<typename T>
bool __attribute__ ((warn_unused_result)) TestCase::getConfig(const std::string& jpath, T& value) {
	// nlohmann::json data;
	try {
        nlohmann::json::json_pointer jp{jpath};
        value = m_config.at(jp);
        return true;
	} catch (nlohmann::json::parse_error& ex) {
		// BOOST_LOG_TRIVIAL(fatal) <<"Get \""<<jsonPointer<< "\" config failed: " <<e.what();
        loggers::TESTCASE_MANAGER().error("get config error: {}", ex.what());
		return false;
	} catch (nlohmann::json::out_of_range& ex) {
        loggers::TESTCASE_MANAGER().error("get config error: {}", ex.what());
		// BOOST_LOG_TRIVIAL(fatal) <<"Get \""<<jsonPointer<< "\" config failed: " <<e.what();
		return false;
	} catch (nlohmann::json::type_error& ex) {
        loggers::TESTCASE_MANAGER().error("get config error: {}", ex.what());
		// BOOST_LOG_TRIVIAL(fatal) <<"Get \""<<jsonPointer<< "\" config failed: " <<e.what();
		return false;
	}
}
