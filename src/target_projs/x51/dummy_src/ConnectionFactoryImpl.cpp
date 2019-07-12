#include "../ConnectionFactory.h"
#include <modules/login/shared/events/EventEncryptEvList.h>
#include "X51Connection.h"
#include <boost/log/trivial.hpp>

class ConnectionFactoryImpl : public ConnectionFactory {
public:
    ConnectionFactoryImpl(IEventSelector *selector, const ConnectionParameters& params) : ConnectionFactory{selector, params} {
        CClientBase::SetEventDispatcher(this);
    }
protected:
    // 这个函数是需要子类重写的
    bool DispatchEvent(IEventLink * link, IEvent * event) override {
        // 处理加密消息
        if (processEncryptEvent(event)) {
            return true;
        }
        
        // 处理一下分片问题
        CBiboPtr<IEvent> splicedEvent;
        m_fragmentManager.SpliceEvent(link, event, &splicedEvent);
        if (splicedEvent) {
            return dispatchRealEvent(link, splicedEvent);
        } else {
            return dispatchRealEvent(link, event);
        }
    }

    virtual bool processEncryptEvent(IEvent* event) {
        // 这个是x51的
        if (event->GetCLSID() == CEventEncryptEvList::_GetCLSID()) {
            BOOST_LOG_TRIVIAL(debug) << "AddEncryptEvent";
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

    std::shared_ptr<Connection> createConnection(IEventLink* link, const ConnectionParameters &params) override {
        return std::shared_ptr<Connection>{new X51Connection{link, params}};
    }

};

std::shared_ptr<ConnectionFactory> createConnectionFactory(IEventSelector *es, const ConnectionParameters& params) {
    return std::shared_ptr<ConnectionFactory>{new ConnectionFactoryImpl{es, params}};
}