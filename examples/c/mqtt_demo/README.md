# Gtest demo (C/C++)

Esta carpeta contiene un ejemplo de `paho.mqtt.c` ([repo](https://github.com/eclipse/paho.mqtt.c), [docs](https://www.eclipse.org/paho/files/mqttdoc/MQTTClient/html/index.html)) con tests unitarios de `gtest`.

## Pasos de construcción
Desde el contenedor de desarrollo en el directorio base del ejemplo (`examples/c/mqtt_demo/`) ejecute los siguiente comandos:

```console
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Note que este comando `make` default no construye los test unitarios.

## MQTT broker
Desde su máquina local (fuera del contenedor de desarrollo) en el directorio base del ejemplo, puede levantar el broker de MQTT (`mosquitto`) utilizando el siguiente comando:

```console
$ docker run -it --name mosquitto -p 1883:1883 -p 9001:9001 -v $(pwd)/mosquitto.conf:/mosquitto/config/mosquitto.conf eclipse-mosquitto
```

Para verificar el funcionamiento de `mosquitto` puede utilizar los programas `mosquitto_pub` y `mosquitto_sub`. Su instalación en Ubuntu se puede realizar con el siguiente comando:

```console
$ sudo apt install mosquitto-clients
```

Por ejemplo, para publicar un mensaje de prueba en un topic llamado `test_topic`. 

```console
$ mosquitto_pub -h 172.17.0.1 -t test_topic -m "Hello!"
```

Para subscribirse e imprimir mensajes provenientes de un topic llamado `test_topic`.

```console
$ mosquitto_sub -h 172.17.0.1 -t test_topic
```

Note que la dirección IP puede cambiar, si se ejecuta desde dentro del contenedor puede ser la dirección del host dada por la interfaz `docker0`.

## Tests unitarios

Los componentes relacionados a los tests unitarios se encuentra en la carpeta `test`. Tanto el framework de `googletest` como `paho.mqtt.c`, se descargan al configurarse el proyecto con CMake, el cual se enlaza al ejecutable `unittest`. Los programas de tests se encuentran en la carpeta `test/tests`.

Para construir los tests unitarios puede correr el siguiente comando desde la carpeta build de CMake:

```console
$ make unittest
```

### Ejecución de tests

Para lanzar todos los tests en el ejecutable de `unittest` puede correr el siguiente comando:

```console
$ make run_unittest
```

Note que al ejecutar el test desde del contenedor de desarrollo se debe ajustar la dirección del broker a la IP del host de Docker (en lugar de `localhost`). Puede obtener esta dirección revisando la interfaz de red `docker0`, desde su máquina local (fuera del contenedor).

Para pasar argumentos extra al lanzar el ejecutable de `unittest` puede utilizar la variable `GTEST_ARGS`:

```console
$ make run_unittest GTEST_ARGS="--gtest_filter=mqtt_client_fixture.* --gtest_random_seed=10 --gtest_repeat=5"
```
