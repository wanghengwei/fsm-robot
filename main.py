import yaml
import os.path
from jinja2 import Template, Environment, FileSystemLoader, select_autoescape

with open('states.yaml') as f:
    data = list(yaml.load_all(f, Loader=yaml.Loader))

env = Environment(loader=FileSystemLoader('.'), autoescape=None)

srcs = []
headers = []
states = []

for d in data:
    d = d['state']
    print(d['name'])
    d['classname'] = f'State{d["name"]}'
    filename = f'state_{d["name"]}'
    d['filename'] = filename
    if 'timeout' not in d:
        d['timeout'] = 10000
    if 'wait' not in d:
        d['wait'] = True
    t = env.get_template('example.h.tpl')
    s = t.render(d)
    with open(f'states/{filename}.h', 'w') as f:
        f.write(s)
    t = env.get_template('example.cpp.tpl')
    s = t.render(d)
    with open(f'states/{filename}.cpp', 'w') as f:
        f.write(s)
    srcs.append(f'{filename}.cpp')

    if not os.path.exists(f'states/{filename}_impl.cpp'):
        t = env.get_template('example_impl.cpp.tpl')
        s = t.render(d)
        with open(f'states/{filename}_impl.cpp', 'w') as f:
            f.write(s)

    srcs.append(f'{filename}_impl.cpp')
    headers.append(f'states/{filename}.h')
    states.append(d['name'])

t = env.get_template('CMakeLists.txt.tpl')
s = t.render(srcs=srcs)
with open('states/CMakeLists.txt', 'w') as f:
    f.write(s)

t = env.get_template('state_factory.cpp.tpl')
s = t.render(headers=headers, states=states)
with open('state_factory.cpp', 'w') as f:
    f.write(s)