#include "state_CEventLogin.h"
#include <QtCore/QDebug>

namespace state {

    void StateCEventLogin::perform() {
        // todo
        qDebug() << "send CEVentLogin";
        QTimer::singleShot(1000, this, &StateCEventLogin::ev_CEventLoginRes_ok);        
    }

}