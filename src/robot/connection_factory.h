#pragma once
#include <memory>

class BasicConnection;

class BasicConnectionFactory {
public:
    virtual ~BasicConnectionFactory() {}
    virtual std::unique_ptr<BasicConnection> create(int connType) = 0;
};

class FakeConnectionFactory : public BasicConnectionFactory {
public:
    std::unique_ptr<BasicConnection> create(int connType) override;
};
