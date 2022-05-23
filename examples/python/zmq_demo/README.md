# ZMQ demo (Python)

Esta carpeta contiene programas de ejemplo del paquete ZeroMQ para Python (PyZMQ).

## Pasos de construcción
Desde el contenedor de desarrollo ejecute los siguiente comandos para generar un ambiente virtual e instalar el paquete de Python del ejemplo:

```console
$ python3 -m venv .venv
$ source .venv/bin/activate
$ pip3 install pyzmq
```

Puede ejecutar los scripts directamente utilizando ``python3``.

## Ejemplo ZMQ REQ client

Este ejemplo ejercita un patrón sencillo de solicitudes y respuestas (REQ/REP).

Este script (`zmq_client_req.py`) corresponse al cliente (socket ZMQ_REQ) para el programa `zmq_server_rep_pthread` en `examples/c/zmq_demo` (socket ZMQ_REP).
