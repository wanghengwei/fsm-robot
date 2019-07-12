#pragma once
#include <share/network/eventframe/ClientBase.h>
#include <share/game_event/EventFragmentManager.h>
#include <functional>
#include <unordered_map>

using ConnectCallback = std::function<void(bool ok, const std::string& errmsg, IEventLink*)>;

namespace x51 {

    struct ConnectionParameters {
        bool isTrusted{true};
        bool skipHandshake{true};
        bool shouldWrap{false}; // 是否要wrap消息。mgc专用的
    };

    class NetClient final : protected CClientBase, protected IEventDispatcher {
    public:
        NetClient(IEventSelector *selector, const ConnectionParameters& params);

        void connect(const std::string& ip, int port, const std::string& id, ConnectCallback f);
    private:
        void OnNewLink(IEventLink* newlink) override;
	    void OnConnFail(INetConnection* conn) override;
	    void OnConnClose(IEventLink* link) override;
        bool DispatchEvent(IEventLink* link, IEvent* event) override;
        bool processEncryptEvent(IEvent* event);
        bool dispatchRealEvent(IEventLink* link, IEvent* event);
    private:
        std::unordered_map<intptr_t, ConnectCallback> m_callbacks;
    };
}