#pragma once
#include "basic_connection.h"
#include "connection_factory.h"

class GameConnection final : public BasicConnection {
private:
    ConnectionFactory& factory() override {
        static ConnectionFactory fac{true};
        return fac;
    }
};
