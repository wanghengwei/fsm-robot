#include "state_CEventHeart.h"
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

namespace state {

    void StateCEventHeart::perform() {
        // todo
        // throw "todo";
        qDebug() << QDateTime::currentDateTime() << "heart!";
    }

}