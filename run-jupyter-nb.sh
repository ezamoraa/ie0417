#!/bin/bash
ip=$(ip addr show eth0 | grep inet | awk '{ print $2 }' | cut -d / -f 1)
JUPYTER_CONFIG_DIR=/home/dev/ws/.jupyter jupyter-notebook --ip=$ip
