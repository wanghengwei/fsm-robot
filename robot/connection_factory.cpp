#include "connection_factory.h"
#include "basic_connection.h"

std::unique_ptr<BasicConnection> FakeConnectionFactory::create(int connType) {
    return std::unique_ptr<BasicConnection>{new FakeConnection};
}
