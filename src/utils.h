#pragma once
#include <random>
#include <nlohmann/json.hpp>
// #include <boost/lexical_cast.hpp>
// #include <QVariant>

// using nlohmann::json;

// // 让json可以被放入QVariant中
// Q_DECLARE_METATYPE(json)

// /**
//  * 从json转换成QString，给json的自动转换用的，一般不自己调
//  * */
// void from_json(const json& j, QString& s);

// /**
//  * 从json转换成QVariant，给json的自动转换用的，一般不自己调
//  * */
// void from_json(const json& j, QVariant& s);

namespace utils {
    /**
     * 从一个json数组中随机选取一个元素返回
     * 
     * @return 如果ja不是数组，就直接返回ja本身；如果ja是空，那么返回一个空的json对象
     * */
nlohmann::json sample(const nlohmann::json& ja) {
    if (ja.is_array()) {
        if (ja.empty()) {
            return nlohmann::json{};
        }
        int index = rand() % ja.size();
        return ja[index];
    } else {
        return ja;
    }
}


    // template<typename T>
    // std::string to_string(const T& v) {
    //     return boost::lexical_cast<std::string>(v);
    // }

    // inline std::string to_string(char c) {
    //     return to_string(int(c));
    // }

    // std::string to_string(QString s);

    // template<typename K, typename V>
    // std::string to_string(const std::map<K, V>&) {
    //     return "MAP_CONTENT";
    // }

    //random int
inline int gen_random(int minimum = 0, int limits = 0x7fffffff)
{
    std::mt19937 rand{std::random_device{}()};
    std::uniform_int_distribution<int> dist(minimum,limits);
    return dist(rand);
}
// 	namespace mgc {
// 		inline long long genPlayerId(const QString &account, int zoneId,int sex = 0,long long channel= 0)
// 		{
// 			long long id = 0;
// 			long long channelId = channel;
// 			channelId &= 0x0F;
// 			channelId <<= 59;

// 			long long zone = zoneId % 10000;
// 			zone &= 0x0FFFF;
// 			zone <<= 43;
// 			long long qq = account.toLongLong();
// 			sex = qq % 2;
// 			long long gender = sex;
// 			gender &= 0x01;
// 			gender <<= 32;

// 			qq &= 0x0FFFFFFFF;
// 			id = channel | zone | gender | qq;
// 			return id;
// 		}
// 	}
// }

/**
 * 从一个json数组中随机抽取一个赋值给t
 * 
 * 如果ja不是一个数组，那么就直接把ja赋值给t
 * 
 * 注意类型要对应，不会执行类型转换，比如str到int
 * */
template<typename T>
void random_get_to(const nlohmann::json& ja, T& t) {
    utils::sample(ja).get_to(t);
}

// QString qvariant_to_qstring(const QVariant& v);
// std::string qvariant_to_string(const QVariant& v);

// std::string dump_string(QVariantMap v);
// std::string dump_string(const std::map<std::string, std::string>& m);

template<typename T, typename It>
void sample(It begin, It end, T& t) {
   // static auto rd = std::random_device{}();
   // std::vector<T> out;
   // std::sample(begin, end, std::back_inserter(out), 1, std::mt19937{rd});
   // t = out[0];
	int idx = rand() % std::distance(begin,end);
	std::advance(begin, idx);
	t = *begin;
}

std::string generateRandomString(int size);

}