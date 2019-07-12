#pragma once
#include "basic_connection.h"
#include <memory>

struct IEvent;
struct IEventLink;

namespace x51 {
    class NetClient;

    class Connection : public BasicConnection {
        friend class NetClient;
    public:
        ~Connection();
        
        void setNetClient(std::shared_ptr<NetClient> p) {
            m_netClient = p;
        }

        void connect(const std::string& ip, int port, const std::string& id) override;
        void close() override;
        void sendEvent(void* e) override;
    private:
        void onEvent(IEvent *ev);

        // 连接被服务器主动关闭时调用此函数
        void closedByServer();
        NetClient& netClient();
    private:
        std::shared_ptr<NetClient> m_netClient;
        IEventLink* m_link = nullptr;
    };
}
