#include "x51_connection.h"
#include "x51_netclient.h"
#include <QtCore/QPointer>
#include <logger.h>

namespace x51 {

    Connection::~Connection() {
        this->close();
    }

    NetClient& Connection::netClient() {
        Q_ASSERT_X(m_netClient, __PRETTY_FUNCTION__, "net client is null");
        return *m_netClient;
    }

    void Connection::connect(const std::string& ip, int port, const std::string& id) {
        if (m_link != nullptr) {
            return;
        }

        QPointer<Connection> p = this;
        netClient().connect(ip, port, id, [=](bool ok, const std::string& errmsg, IEventLink* link) {
            loggers::NET().trace("on connect callback: ok={}, errmsg={}", ok, errmsg);
            if (!p) {
                loggers::NET().trace("this pointer of Connection is null, do not emit event");
                return;
            }
            if (ok) {
                p->m_link = link;
                p->m_link->SetPtr(this);
                Q_EMIT p->connectOK();
            } else {
                Q_EMIT p->connectFailed(errmsg);
            }
        });
    }

    void Connection::close() {
        if (m_link != nullptr) {
            m_link->SetPtr(nullptr);
            m_link->Close("by-user");
            m_link = nullptr;
        }
    }

    void Connection::sendEvent(void* e) {
        Q_ASSERT_X(m_link != nullptr, __PRETTY_FUNCTION__, "link is null!");
        
        IEvent* ev = static_cast<IEvent*>(e);

        static CEventFragmentManager cfm;
        std::vector<IEvent*> frags;
        if (cfm.SliceEvent(ev, frags)) {
            for (std::vector<IEvent*>::iterator it = frags.begin(); it != frags.end(); ++it) {
                m_link->SendEvent(*it);
                (*it)->Release();
            }
        } else {
            m_link->SendEvent(ev);
        }
    }

    void Connection::onEvent(IEvent *ev) {
        Q_ASSERT_X(ev, __PRETTY_FUNCTION__, "event cannot be null");

        Q_EMIT this->eventReceived(ev);
    }

    void Connection::closedByServer() {
        if (m_link == nullptr) {
            return;
        }

        m_link->SetPtr(nullptr);
        m_link = nullptr;
        Q_EMIT this->kicked();
    }
}