#pragma once

#include <basic_state.h>

class BasicConnection;

class SendWaitEventState : public BasicState {
    Q_OBJECT
public:
    using BasicState::BasicState;

    void perform() override;
    void clean() override;

protected:
    virtual void onEventReceived(void* e) = 0;
    virtual void doSendEvent(BasicConnection& conn) = 0;
    virtual int connectionType() const = 0;
};
