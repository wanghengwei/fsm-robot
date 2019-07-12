#pragma once
#include "connection_factory.h"

namespace x51 {

    class NetClientManager;

    class ConnectionFactory : public BasicConnectionFactory {
    public:
        ConnectionFactory(std::shared_ptr<NetClientManager> p);

        // void setNetClientManager() {
        //     m_ncm = p;
        // }

        std::unique_ptr<BasicConnection> create(int connType) override;

    private:
        NetClientManager& netClientManager();
    private:
        std::shared_ptr<NetClientManager> m_ncm;
    };

}