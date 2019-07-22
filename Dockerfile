FROM ubuntu:18.04

RUN echo 'deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic main restricted universe multiverse \n\
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-updates main restricted universe multiverse \n\
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-backports main restricted universe multiverse \n\
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ bionic-security main restricted universe multiverse' > /etc/apt/sources.list

RUN apt-get update && apt-get install -y libqt5core5a libpugixml1v5 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

ENV LD_LIBRARY_PATH=/app

COPY bin .