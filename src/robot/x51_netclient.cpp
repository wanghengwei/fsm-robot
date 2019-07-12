#include "x51_netclient.h"
#include "x51_connection.h"
#include <QtCore/QDebug>
#include <modules/login/shared/events/EventEncryptEvList.h>
#include <logger.h>

namespace x51 {
    NetClient::NetClient(IEventSelector *selector, const ConnectionParameters& params) : CClientBase(
        selector,
        INetConnection::PKPROTO_BASE_UTIL,
        params.isTrusted,
        !params.isTrusted,
        nullptr,
        1024 * 1024,
        false,
        PT_POLLING,
        IO_MAX_CONNECTION
    ) {
        INetInterface::net_config_t cfg = GetNetInterface()->GetConfig();
        cfg.m_max_conn_per_IP = 30000;
        cfg.m_skip_handshake = params.skipHandshake ? 1 : 0;
        GetNetInterface()->SetConfig(cfg);
        this->CClientBase::SetEventDispatcher(this);
    }

    void NetClient::connect(const std::string& ip, int port, const std::string& id, ConnectCallback f) {
        INetConnection* c = this->CClientBase::Connect(ip.c_str(), port, id.c_str(), "");
        auto key = reinterpret_cast<intptr_t>(c);
        m_callbacks.emplace(key, std::move(f));
    }

    void NetClient::OnConnFail(INetConnection *conn) {
        // find out the Connection object
        Q_ASSERT_X(conn != nullptr, __PRETTY_FUNCTION__, "conn is null!");

        auto k = reinterpret_cast<intptr_t>(conn);
        auto it = m_callbacks.find(k);
        if (it == m_callbacks.end()) {
            Q_ASSERT_X(false, __PRETTY_FUNCTION__, "cannot find key!");
            return;
        }

        ConnectCallback f = std::move(it->second);
        m_callbacks.erase(it);

        f(false, conn->GetErrorString(), nullptr);
    }

    void NetClient::OnNewLink(IEventLink *newlink) {
        loggers::NET().trace("{}", __PRETTY_FUNCTION__);

        auto rconn = newlink->GetConnection();
        std::string acc = rconn->GetAccountName();

        auto k = reinterpret_cast<intptr_t>(rconn);
        auto it = m_callbacks.find(k);
        Q_ASSERT_X(it != m_callbacks.end(), __PRETTY_FUNCTION__, "new link but no corresponding connection");

        auto f = std::move(it->second);
        m_callbacks.erase(it);

        f(true, "", newlink);

        // auto pconn = createConnection(newlink, m_params);

        // newlink->SetPtr(pconn.get());

        // p.set_value(pconn);
    }

    void NetClient::OnConnClose(IEventLink* link) {
        void* p = link->GetPtr();
        if (p == nullptr) {
            // 如果是null，那应该是相应的connection对象已经析构了。
            return;
        }

        Connection* pcon = static_cast<Connection*>(p);
        pcon->closedByServer();
    }

    bool NetClient::DispatchEvent(IEventLink* link, IEvent* event) {
        // 处理加密消息
        if (this->processEncryptEvent(event)) {
            return true;
        }
        
        // 处理一下分片问题
        CBiboPtr<IEvent> splicedEvent;
        static CEventFragmentManager cfm;
        cfm.SpliceEvent(link, event, &splicedEvent);
        if (splicedEvent) {
            return dispatchRealEvent(link, splicedEvent);
        } else {
            return dispatchRealEvent(link, event);
        }
    }

    bool NetClient::processEncryptEvent(IEvent* event) {
        // 这个是x51的
        if (event->GetCLSID() == CEventEncryptEvList::_GetCLSID()) {
            loggers::NET().trace("process CEventEncryptEvList");
            auto encryptEvent = static_cast<CEventEncryptEvList*>(event);
            const auto& events = encryptEvent->GetEncryptList();
            auto netInterface = this->GetNetInterface();
            for (auto& e : events) {
                if (e.direction == 2) {
                    netInterface->AddEncryptEventID(e.eventid, 0);
                    netInterface->AddEncryptEventID(e.eventid, 1);
                } else {
                    netInterface->AddEncryptEventID(e.eventid, e.direction);
                }
            }

            return true;
        }

        return false;
    }

    // 处理最终的真实的event，通用的函数
    bool NetClient::dispatchRealEvent(IEventLink* link, IEvent* event) {
        std::string acc = link->GetConnection()->GetAccountName();

        // 处理收到的event
        // BOOST_LOG_TRIVIAL(debug) << "ConnectionFactory::dispatchRealEvent: link=" << link << ", real_event=" << event->GetRealName() << ", event_name=" << event->GetEventName() << ", clsid=" << event->GetCLSID() << ", acc=" << acc;

        auto conn = static_cast<Connection*>(link->GetPtr());

        // 这里如果是null，说明对应的conn析构了
        if (conn == nullptr) {
            // BOOST_LOG_TRIVIAL(debug) << "dispatch event but conn is null. link={:x}, conn={:x}, acc={}";//_format(intptr_t(link), intptr_t(link->GetConnection()), acc);
            return false;
        }

        // 让每个机器人的连接去自己处理自己的消息
        conn->onEvent(event);

        // BOOST_LOG_TRIVIAL(trace) << "event " << event->GetRealName() << " will be destroyed";
        return true;
    }
}