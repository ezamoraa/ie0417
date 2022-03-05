Getting Started
===============

The development environment is meant to run on any platform where Linux
containers are supported, but has been actively tested on MacOS (with Docker
Desktop), and Ubuntu Linux hosts.

Requirements
------------

- **Vagrant** to control the development environment, please follow install
  `instructions for your platform here <https://www.vagrantup.com/downloads.html>`_.
- **Docker** to provide a lightweight container based environment for vagrant.
  Please follow the `install instructions for your platform <https://docs.docker.com/install/>`_.
- **Git** client to be able to clone the repository

Cloning and Starting
--------------------

Clone the repository from GitHub, then start the vagrant environment

.. code-block:: bash

   git clone https://github.com/ezamoraa/ie0317
   cd ie0317
   vagrant up

.. tip::

   This process may take some time the first time, as Docker will start downloading
   the container image from the public docker registry hub.

Once the Vagrant machine is up and running, you can establish a secure remote
connection via ssh by running `vagrant ssh` and the output should look like:

.. code-block:: bash

  $ vagrant ssh
  Welcome to Ubuntu 20.04.3 LTS (GNU/Linux 5.15.21-1-linux x86_64)

  $$$$$$\ $$$$$$$$\        $$$$$$\   $$$$$$\    $$\   $$$$$$$$\
  \_$$  _|$$  _____|      $$$ __$$\ $$ ___$$\ $$$$ |  \____$$  |
    $$ |  $$ |            $$$$\ $$ |\_/   $$ |\_$$ |      $$  /
    $$ |  $$$$$\          $$\$$\$$ |  $$$$$ /   $$ |     $$  /
    $$ |  $$  __|         $$ \$$$$ |  \___$$\   $$ |    $$  /
    $$ |  $$ |            $$ |\$$$ |$$\   $$ |  $$ |   $$  /
  $$$$$$\ $$$$$$$$\       \$$$$$$  /\$$$$$$  |$$$$$$\ $$  /
  \______|\________|       \______/  \______/ \______|\__/

        IE0317 development environment 22.03.0

  Last login: Mon Feb 28 04:26:37 2022 from 172.17.0.1
  dev@ie0317-devel:~/ws$

.. note::

   The vagrant is configured to sync some folders from your host machine into
   your development machine, in special the folder with the git repo is available
   at `~/ws` and that is your default folder when doing ssh.

.. tip::

   You can open as many SSH sessions as you want. Some code editors like Visual
   Studio Code support `ssh remote edition <https://code.visualstudio.com/docs/remote/ssh>`_, which is very convenient.

Life cycle of the development machine
-------------------------------------

Once you are done with your development, you may want to "turn off" the container
with `vagrant halt`.

You may also need to destroy the environment altogether (for example to upgrade
to a new version of the environment), and you can do that with `vagrant destroy`.