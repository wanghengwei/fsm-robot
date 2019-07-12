#pragma once
#include <memory>
#include <map>

struct IEventSelector;

namespace x51 {
    class NetClient;

    class NetClientManager {
    public:
        NetClientManager(IEventSelector* es);

        std::shared_ptr<NetClient> get(int connType);
    private:
        std::map<int, std::shared_ptr<NetClient>> m_netclients;
    };
}