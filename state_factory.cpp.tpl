#include "state_factory.h"
#include "idle.h"
{% for h in headers %}
#include "{{ h }}"
{% endfor %}

QState* createStateByID(QString id, QState* parent) {
    if (id == "Idle") {
        return new Idle{parent};
    }

    {% for s in states %}
    if (id == "{{ s }}") {
        return new state::State{{ s }}{parent};
    }
    {% endfor %}

    return nullptr;
}