#pragma once
#include <memory>

class BasicConnection;

class ConnectionFactory {
public:
    virtual ~ConnectionFactory() {}
    virtual std::unique_ptr<BasicConnection> create(int connType) = 0;
};

class FakeConnectionFactory : public ConnectionFactory {
public:
    std::unique_ptr<BasicConnection> create(int connType) override;
};
