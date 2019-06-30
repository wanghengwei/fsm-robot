#pragma once
#include <QtCore/QStateMachine>
#include <QtCore/QTimer>
#include "robot.h"

class TestCase {
public:

    template<typename S>
    S* createState() {
        S* p = new S{};
        p->init();
        p->setRobot(m_robot);
        return p;
    }

    void start();

private:
    QStateMachine m_machine;
    Robot m_robot;
};