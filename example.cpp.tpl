#include "{{ filename }}.h"

namespace state {
    {{ classname }}::{{ classname }}(QState* parent) : BaseState{parent} {
        {% if wait %}
        setTimeout({{ timeout }});
        QObject::connect(this->timer(), &QTimer::timeout, this, &BaseState::ev_timeout);
        {% else %}
        setTimeout(-1);
        {% endif %}
    }
}