# (C) Copyright 2020-2022 Hewlett Packard Enterprise Development LP.

FROM ubuntu:focal-20220113 as base

ENV DEBIAN_FRONTEND noninteractive
ENV DEBCONF_NONINTERACTIVE_SEEN=true

# Setup HTTP proxies (will be used to fetch code and binaries)
ARG http_proxy=
ENV http_proxy=$http_proxy
ARG https_proxy=
ENV https_proxy=$https_proxy
ARG no_proxy=
ENV no_proxy=$no_proxy

# Install base system software
RUN apt-get update && apt-get --yes --no-install-recommends install \
        curl git locales sudo iproute2 locales

# Set the locale
ENV LANG en_US.UTF-8
RUN locale-gen en_US.UTF-8 && update-locale LANG=en_US.UTF-8

# Install SSH Server
RUN apt-get update && apt-get --yes --no-install-recommends install \
        openssh-server && \
    mkdir -p /var/run/sshd && \
    chmod 0755 /var/run/sshd && \
    echo "StrictHostKeyChecking no" >> /etc/ssh/ssh_config && \
    sed -i 's/AcceptEnv/# AcceptEnv/' /etc/ssh/sshd_config

# Setup to work with Vagrant: add an user 'dev' and run ssh daemon on the container

# Create and configure user
RUN echo "Creating dev user" && \
    useradd --create-home -s /bin/bash dev && \
    mkdir -p /home/dev/.ssh && \
    \
    echo "Changing dev user password" && \
    echo -n 'dev:dev' | chpasswd && \
    \
    echo "Enable passwordless sudo for the dev user" && \
    echo 'dev ALL=NOPASSWD: ALL' > /etc/sudoers.d/dev && \
    \
    echo "Configuring ssh keys" && \
    echo "ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEA6NF8iallvQVp22WDkTkyrtvp9eWW6A8YVr+kz4TjGYe7gHzIw+niNltGEFHzD8+v1I2YJ6oXevct1YeS0o9HZyN1Q9qgCgzUFtdOKLv6IedplqoPkcmF0aYet2PkEDo3MlTBckFXPITAMzF8dJSIFo9D8HfdOV0IAdx4O7PtixWKn5y2hMNG0zQPyUecp4pzC6kivAIhyfHilFR61RGL+GPXQ2MWZWFYbAGjyiYJnAmCP3NOTd0jMZEnDkbUvxhMmBYSdETk1rRgm+R4LOzFUGaHqHDLKLX+FIPKcF96hrucXzcWyLbIbEgE98OHlnVYCzRdK8jlqm8tehUc9c9WhQ== Vagrant insecure public key" > /home/dev/.ssh/authorized_keys && \
    chown -R dev: /home/dev/.ssh && \
    ssh-keyscan -H github.com > /home/dev/.ssh/known_hosts && \
    \
    echo "Setting correct permissions to the workspace" && \
    mkdir -p /home/dev/ws && chown -R dev: /home/dev/ws

# Launch SSH daemon
EXPOSE 22
CMD /usr/sbin/sshd -D -o UseDNS=no

# Install developer tools
RUN apt-get update && apt-get --yes --no-install-recommends install \
    python3 python3-setuptools python3-pip \
    vim nano tree ack-grep bash-completion less \
    build-essential cmake pkg-config libtool automake \
    lcov gdb gdbserver valgrind \
    doxygen graphviz strace ltrace wget curl rsync \
    libevent-dev libjson-c-dev

# Install packages for python
COPY files/requirements.txt requirements.txt
RUN python3 -m pip install -r requirements.txt && rm requirements.txt
RUN jupyter contrib nbextension install

# Update MOTD info
RUN rm /etc/update-motd.d/10-help-text /etc/update-motd.d/50-motd-news /etc/update-motd.d/60-unminimize
COPY version /etc/env_version
COPY files/motd/static /etc/update-motd.d/static
COPY files/motd/version /etc/update-motd.d/10-version
