#pragma
#include <QtCore/QObject>
#include <map>
#include <memory>
#include "basic_connection.h"

class ConnectionFactory;

class BasicRobot : public QObject {
    Q_OBJECT
public:
    void setConnectionFactory(std::shared_ptr<ConnectionFactory> cf) {
        m_connectionFactory = cf;
    }

    BasicConnection& connection(int connType);

// signals:
//     void connectOK();
//     void connectFailed();
private:
    std::map<int, std::unique_ptr<BasicConnection>> m_conns;
    std::shared_ptr<ConnectionFactory> m_connectionFactory;
};