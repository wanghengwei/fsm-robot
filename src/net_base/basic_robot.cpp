#include "basic_robot.h"
#include "connection_factory.h"
#include "basic_connection.h"

BasicConnection& BasicRobot::connection(int connType) {
    auto it = m_conns.find(connType);
    if (it != m_conns.end()) {
        return *it->second;
    }

    Q_ASSERT_X(m_connectionFactory != nullptr, __PRETTY_FUNCTION__, "connection factory is null!");

    std::unique_ptr<BasicConnection> c = m_connectionFactory->create(connType);

    BasicConnection& r = *c;
    m_conns.emplace(connType, std::move(c));
    return r;
}