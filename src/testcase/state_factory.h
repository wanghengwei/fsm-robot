#pragma once
#include <QtCore/QState>

extern QState* createStateByID(QString id, QState* parent = nullptr);