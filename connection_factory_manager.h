#pragma once
#include "connection_factory.h"

class ConnectionFactoryManager {
public:
    ConnectionFactory& game() { return m_game; }
    ConnectionFactory& match() { return m_match; }
private:
    ConnectionFactory m_game{true};
    ConnectionFactory m_match{false};
};
