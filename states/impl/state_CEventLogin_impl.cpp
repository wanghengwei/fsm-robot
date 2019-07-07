
#include "state_CEventLogin.h"
#include <stdexcept>
#include <QtCore/QTimer>
#include <logger.h>

namespace state {

    void StateCEventLogin::perform(std::map<std::string, std::string>& info) {
        // todo
        // throw std::runtime_error{"todo"};
        int zone = 0;
        GET_DATA_OR_DIE("zone", zone);
        info["zone"] = std::to_string(zone);
        QTimer::singleShot(1000, this, [=]() {
            emit this->ev_CEventLoginRes_ok();
        });
    }

}
