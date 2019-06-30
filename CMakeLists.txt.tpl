set(SRCS
{% for src in srcs %}
{{ src }}
{% endfor %}
)

add_library(states ${SRCS})
target_link_libraries(states Qt5::Core)