#include "x51_connection_factory.h"
#include "x51_connection.h"
#include "x51_netclient_manager.h"

namespace x51 {
    ConnectionFactory::ConnectionFactory(std::shared_ptr<NetClientManager> p) : m_ncm(p) {

    }

    std::unique_ptr<BasicConnection> ConnectionFactory::create(int connType) {
        auto p = new Connection;
        p->setNetClient(netClientManager().get(connType));
        return std::unique_ptr<BasicConnection>{p};
    }

    NetClientManager& ConnectionFactory::netClientManager() {
        Q_ASSERT_X(m_ncm, __PRETTY_FUNCTION__, "netclientmanager is null");
        return *m_ncm;
    }
}