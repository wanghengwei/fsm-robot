#pragma once
#include <QtCore/QState>

QState* createStateByID(QString id, QState* parent = nullptr);