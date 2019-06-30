#include "basic_connection.h"
#include "connection_factory.h"

void BasicConnection::connect() {
    factory().connect(account());
}