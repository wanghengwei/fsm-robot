#pragma once
#include <QtCore/QObject>
#include <QtCore/QState>
#include <QtCore/QTimer>

class ActAndWait : public QObject {
    Q_OBJECT
public:
    explicit ActAndWait(std::function<void()> act, int ms);

    void setTimeoutTransition(QAbstractState* s);

    template<typename T, typename F>
    void addTransition(T* t, F f, QAbstractState* s) {
        state.addTransition(t, f, s);
    }

    QState state;
    QTimer timer;
};