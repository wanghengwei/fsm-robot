#include "../ConnectionFactoryManager.h"
#include "../ConnectionFactory.h"

class ConnectionFactoryManagerImpl : public ConnectionFactoryManager {
public:

    ConnectionFactoryManagerImpl() {}

    void setup(IEventSelector *es) override {
        ConnectionParameters params;

        // game server 
        params.isTrusted = false;
        params.skipHandshake = false;
        params.shouldWrap = false;
        m_factories[0] = createConnectionFactory(es, params);

        // match server
        params.isTrusted = true;
        params.skipHandshake = false;
        params.shouldWrap = false;
        m_factories[1] = createConnectionFactory(es, params);

        // global community server
        params.isTrusted = true;
        params.skipHandshake = false;
        params.shouldWrap = false;
        m_factories[2] = createConnectionFactory(es, params);
    }
};

std::shared_ptr<ConnectionFactoryManager> createConnectionFactoryManager(){
    return std::shared_ptr<ConnectionFactoryManager>(new ConnectionFactoryManagerImpl{});
}
