#include "x51_netclient_manager.h"
#include <QtCore/QDebug>
#include "x51_netclient.h"
#include "connection_types.h"

namespace x51 {
    NetClientManager::NetClientManager(IEventSelector* es) {
        ConnectionParameters params;

        // game server 
        params.isTrusted = false;
        params.skipHandshake = false;
        params.shouldWrap = false;
        m_netclients[CONN_GAME] = std::shared_ptr<NetClient>{new NetClient{es, params}};
    }

    std::shared_ptr<NetClient> NetClientManager::get(int connType) {
        auto it = m_netclients.find(connType);
        Q_ASSERT_X(it != m_netclients.end(), __PRETTY_FUNCTION__, "forgot write netclient?");
        return it->second;
    }
}
