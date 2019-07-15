#pragma once
#include <QtCore/QStateMachine>
#include <QtCore/QDebug>
#include <any>
#include <type_traits>
#include <nlohmann/json_fwd.hpp>

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
    void setData(const nlohmann::json& data);

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
    bool __attribute_warn_unused_result__ getData(const std::string& key, T& value);

    /** 
     * @brief 插入或更新用户数据
    */
    void insertOrUpdateData(const std::string& key, const std::any& value) {
        m_data[key] = value;
    }

    /**
     * @brief 插入数据
     * 
     * @return 成功插入返回true
     * 
     * 如果key已存在，则不插入
     * 
    */
    bool insertData(const std::string& key, const std::any& value) {
        auto r = m_data.insert(std::make_pair(key, value));
        return r.second;
    }

    BasicRobot& robot();

private:
    std::string m_userId;

    // 保存k-v结构的用户数据
    std::map<std::string, std::any> m_data;

    BasicRobot* m_robot = nullptr;
};

namespace impl {
    template<typename T, typename Enabled = void>
    struct GetValueFromAny {
        bool operator()(const std::any& a, T& v) {
            try {
                v = std::any_cast<T>(a);
                return true;
            } catch (std::bad_any_cast const& ex) {
                return false;
            }
        }
    };

    template<typename T>
    struct GetValueFromAny<T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>> {
        bool operator()(const std::any& a, T& v) {
            try {
                // qDebug() << "111";
                v = T(std::any_cast<int64_t>(a));
                return true;
            } catch (std::bad_any_cast const& ex) {
                return false;
            }
        }
    };

    template<typename T>
    struct GetValueFromAny<T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>>> {
        bool operator()(const std::any& a, T& v) {
            try {
                // qDebug() << "111";
                v = T(std::any_cast<uint64_t>(a));
                return true;
            } catch (std::bad_any_cast const& ex) {
                return false;
            }
        }
    };

    template<>
    struct GetValueFromAny<bool> {
        bool operator()(const std::any& a, bool& v) {
            try {
                v = std::any_cast<bool>(a);
                return true;
            } catch (std::bad_any_cast const& ex) {
                return false;
            }
        }
    };
}


template<typename T>
bool __attribute_warn_unused_result__ TestCase::getData(const std::string& key, T& value) {
    auto it = m_data.find(key);
    if (it == m_data.end()) {
        return false;
    }

    // try {
    //     value = std::any_cast<T>(it->second);
    //     return true;
    // } catch (std::bad_any_cast const& ex) {
    //     qDebug() << "cannot get data with key" << key << ":" << ex.what();
    //     return false;
    // }

    return impl::GetValueFromAny<T>{}(it->second, value);
}

