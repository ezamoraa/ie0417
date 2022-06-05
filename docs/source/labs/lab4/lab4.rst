**********************************************
Laboratorio 4: Unit Testing en Python y C/C++
**********************************************

Requisitos previos
==================
* Siga los requisitos previos de `Laboratorio 1 <../lab1/lab1.html>`_ para preparar el ambiente para este laboratorio.

Introducción
============

El objetivo de este laboratorio consiste en llevar acabo las pruebas unitarias de ciertos componentes de los ejemplos `examples/c/sensor_commands <https://github.com/ezamoraa/ie0417/tree/main/examples/c/sensor_commands>`_ y `examples/python/sensor_commands <https://github.com/ezamoraa/ie0417/tree/main/examples/python/sensor_commands>`_ del curso, utilizando ``googletest`` y ``pytest``, respectivamente.

Para ejemplos de ``googletest`` y ``pytest``, refiérase a `examples/c/gtest_demo <https://github.com/ezamoraa/ie0417/tree/main/examples/c/gtest_demo>`_ y `examples/python/pytest_demo <https://github.com/ezamoraa/ie0417/tree/main/examples/python/pytest_demo>`_.

Instrucciones
=============

A continuación se describirán las instrucciones de los apartados de este laboratorio.

Construcción
------------

Sensor commands [C]
^^^^^^^^^^^^^^^^^^^

* Copie el proyecto de ``examples/c/sensor_commands`` en su repositorio de desarrollo y agregue la infrastructura de CMake para ``googletest``. Para esto puede basarse en los scripts de CMake y programas de ``gtest_demo``.

* Copie e integre la biblioteca ``testutil`` de ``gtest_demo`` en ``sensor_commands``. Esto será necesario para poder utilizar la clase utilitaria de ``rand_gen``, de manera que pueda generar números aleatorios reproducibles con semilla (``--gtest_random_seed``).

Sensor commands [Python]
^^^^^^^^^^^^^^^^^^^^^^^^

* Copie el proyecto de ``examples/python/sensor_commands`` en su repositorio de desarrollo y agregue la infrastructura de ``tox + pytest`` para poder correr los test unitarios con el comando ``tox -e test``. Este comando también debe correr ``flake8`` antes de ``pytest``. Para esto puede basarse en los archivos de configuración y scripts de ``pytest_demo``.

Tests unitarios
---------------

Sensor commands [C]
^^^^^^^^^^^^^^^^^^^
Las pruebas unitarias para la versión de C se centrarán en el ``CommandRunner``. Los tests se deberán ubicar en un archivo ``command_runner.cpp``, dentro de la carpeta ``test/tests``.

* Cree una clase fixture de nombre ``command_runner``. Esta se debe encargar de:

  * Instanciar un generador aleatorio de la clase ``testutil::rand_gen``.
  * Crear el ``CommandRunner`` en el ``SetUp``. Aleatorice de manera reproducible el parámetro de configuración ``q_max_size`` entre ``1`` y ``1000``. Esto significa que con el mismo valor de ``--gtest_random_seed=<val>`` se debe obtener el mismo número aleatorio para ``q_max_size``:
  `` Destruir el ``CommandRunner`` en el ``TearDown``.
  * Debe verificar que tanto la creación como la destrucción del ``CommandRunner`` ocurren de forma exitosa utilizando asserts de ``googletest``.

.. tip::

   Para un ejemplo de un fixture de ``googletest`` y el manejo de la generación aleatoria con semilla reproducible, refiérase a ``demo_api_fixture.cpp`` en ``gtest_demo``.

* Implemente los siguientes tests utilizando el fixture ``command_runner``:

    #. ``command_runner.create_destroy``:

        * Note que la creación y destrucción exitosa es probada en el ``SetUp`` y ``TearDown`` del fixture.
        * Intente crear un command runner con ``NULL cfg``. Verifique que la función falla retornando ``NULL``.
        * Intente destruir un command runner con ``NULL cmd_runner``. Verifique que la función falla retornando ``-1``.

    #. ``command_runner.start_stop``:

        * Intente ejecutar el start del command runner con ``NULL cmd_runner``. Verifique que la función falla retornando ``-1``.
        * Ejecute el start del command runner. Verique que la función tiene éxito.
        * Intente ejecutar el stop del command runner con ``NULL cmd_runner``. Verifique que la función falla retornando ``-1``.
        * Ejecute el stop del command runner. Verique que la función tiene éxito.

    #. ``command_runner.command_send_single``:

        * Ejecute el start del command runner. Verique que la función tiene éxito.
        * Intente enviar un comando con ``NULL cmd_runner``. Verifique que la función falla retornando ``-1``.
        * Intente enviar un comando con ``NULL cmd``. Verifique que la función falla retornando ``-1``.
        * Envíe un comando de tipo ``msg_command`` al command runner. Verifique que la función tiene éxito.
        * Ejecute el stop del command runner. Verique que la función tiene éxito.

Sensor commands [Python]
^^^^^^^^^^^^^^^^^^^^^^^^

Las pruebas unitarias para la versión de Python se centrarán en el ``SensorManager``. Los tests se deberán ubicar en un archivo ``test_sensor_manager.py``, dentro de la carpeta ``test``.

* Cree una función fixture de pytest llamada ``sensor_mgr`` que instancie y retorne un objeto ``SensorManager``. Utilice el archivo de configuración de sensores default del proyecto (``config/sensors_cfg.json``). Los métodos de este objeto ``SensorManager`` serán utilizado en los tests para verificar el funcionamiento del mismo.

* Implemente los siguientes tests utilizando el fixture ``sensor_mgr``:

  #. ``test_sensor_manager_supported_types``:

     * Obtenga la lista de tipos de sensor soportados (``get_supported_sensor_types``). Verifique que la lista no se encuentre vacía.

  #. ``test_sensor_manager_single_sensor_create_destroy``:

     * Cree un sensor con nombre arbitrario y tipo aleatorio válido (temp o level) (``create_sensor``). Verifique que la información del nombre y tipo del sensor se encuentra en el diccionario obtenido con ``get_sensors_info``:
     * Intente crear un sensor con el mismo nombre otra vez. Verifique que se genera una excepción de tipo ``AssertionError`` (refiérase a ``pytest.raises``).
     * Destruya el sensor previamente creado (``destroy_sensor``). Verifique que el nombre del sensor ya no se encuentra en el diccionario de ``gtest_sensors_info``.
     * Intente destruir un sensor con el mismo nombre otra vez. Verifique que se genera una excepción de tipo ``AssertionError``.

  #. ``test_sensor_manager_single_sensor_read_command``:

     * Cree un sensor con nombre arbitrario y tipo aleatorio válido (temp o level) (``create_sensor``).
     * Cree un comando de lectura para este sensor, sin proveer un ``SensorAnalyzer`` (``create_sensor_read_cmd``).
     * Ejecute el comando directamente con su método ``execute`` (sin un command runner).
     * Destruya el sensor previamente creado (``destroy_sensor``).

  #. ``test_sensor_manager_mock_type_register_unregister``:

     * Cree una clase derivada de ``Sensor`` llamada ``MockSensor``. Esta representa un sensor ficticio y será utilizada para guardar información que permita verificar que el sistema interactúa correctamente con el sensor. Además de los métodos estándar de un ``Sensor``, esta clase debe definir un método ``assert_read`` que verifique que el método ``read`` fue llamado al menos una vez, utilizando la directiva ``assert`` de Python. Para esto la implementación de ``read`` debe setear algún tipo de bandera o contador en el objeto que pueda ser utilizada por ``assert_read``. Para más detalles sobre el concepto de ``Mock``, puede referirse a bibliotecas de Python como `unittest.mock <https://docs.python.org/3/library/unittest.mock.html>`_ (no utilizar en este laboratorio).
     * Registre un tipo de sensor llamado ``mock`` con la clase ``MockSensor`` (``register_sensor_type``). Verifique que el nuevo tipo de sensor aparece en la lista de ``get_supported_sensor_types``.
     * Desregistre el tipo de sensor ``mock`` (``unregister_sensor_type``).

  #. ``test_sensor_manager_mock_sensor_create_destroy``:

     * Registre el mismo tipo de sensor ``mock`` del test anterior.
     * Cree un sensor de tipo ``mock`` (``create_sensor``). Verifique que la información del sensor aparece en el diccionario obtenido con ``get_sensors_info``.
     * Destruya el sensor previamente creado (``destroy_sensor``).
     * Desregistre el tipo de sensor ``mock``.

  #. ``test_sensor_manager_mock_sensor_read_command``:

     * Registre el mismo tipo de sensor ``mock`` del test anterior.
     * Cree un sensor de tipo ``mock`` (``create_sensor``). Verifique que la información del sensor aparece en el diccionario obtenido con ``get_sensors_info``.
     * Cree un comando de lectura para este sensor, sin proveer un ``SensorAnalyzer`` (``create_sensor_read_cmd``).
     * Ejecute el comando directamente con su método ``execute`` (sin un command runner). Verifique que el sensor fue leído utilizando el método ``assert_read`` del sensor. Nóte que la instancia del sensor se encuentra encapsulada dentro del ``SensorManager``, sin embargo, para efectos de esta prueba puede obtenerla mediante el atributo ``sensors`` del objeto ``SensorManager``, a partir del nombre del sensor.
     * Destruya el sensor previamente creado (``destroy_sensor``).
     * Desregistre el tipo de sensor ``mock``.

.. note::

    Si al desarrollar las pruebas encuentra algún error en el código de los ejemplos deberá arreglarlo como parte del laboratorio y reportarlo al profesor.

Evaluación
==========
Este laboratorio se realizará preferiblemente en grupos de 2 o 3 personas.

Rúbrica
-------

+---------------------------+------------------------------------------------------------------+------------+
| Apartado                  |  Criterios de evaluación                                         | Porcentaje |
+===========================+==================================================================+============+
| Construcción [C]          || Integrar los scripts de CMake y los programas necesarios        | 10%        |
|                           || para construir el ejecutable de googletest y ejecutar los       |            |
|                           || tests sobre la biblioteca sensor_commands. Integrar la          |            |
|                           || biblioteca testutil y la bandera gtest_random_seed para generar |            |
|                           || números aleatorios reproducibles con semilla.                   |            |
+---------------------------+------------------------------------------------------------------+------------+
| Construcción [Python]     || Integrar los archivos de configuración y scripts necesarios     | 10%        |
|                           || para construir el ambiente virtual de Python con tox y ejecutar |            |
|                           || lost tests sobre el paquete sensor_commands.                    |            |
+---------------------------+------------------------------------------------------------------+------------+
| Tests unitarios [C]       || Lograr implementar los tests unitarios de Googletest            | 30%        |
|                           || siguiendo la secuencia y pasos de verificación descritos        |            |
|                           || en el apartado de instrucciones.                                |            |
|                           || Todas las pruebas pasan consistentemente en al menos            |            |
|                           || 10 repeticiones consecutivas con ``--gtest_repeat=10``.         |            |
+---------------------------+------------------------------------------------------------------+------------+
| Tests unitarios [Python]  || Lograr implementar los tests unitarios de Pytest                | 50%        |
|                           || siguiendo la secuencia y pasos de verificación descritos        |            |
|                           || en el apartado de instrucciones.                                |            |
|                           || Implementar correctamente el método assert_read de MockSensor.  |            |
|                           || Todas las pruebas pasan consistentemente en al menos            |            |
|                           || 10 repeticiones consecutivas con ``--count=10``.                |            |
+---------------------------+------------------------------------------------------------------+------------+

Revisión
--------

* Se debe demostrar con base en los autores de los commits de Git que todos los miembros del grupo trabajaron colaborativamente en el laboratorio utilizando control de versiones. De no ser así, no se asignará puntaje a los miembros que no contribuyeron (a menos que se justifique una excepción).
* Se utilizará la fecha del último commit de `merge` de un Pull Request (PR) de GitHub que modifique la documentación del laboratorio respectivo para determinar si la entrega se realizó a tiempo.
* Si se realizan entregas tardías, se rebajará 10% luego de la hora de entrega oficial y 10% por cada día extra (-10% primer día, -20% segundo día, etc).
* Para los grupos, sólo es necesario subir los cambios en el repositorio de uno de los miembros. Los demás miembros pueden hacer referencia a dicho repositorio y/o sincronizar los cambios en sus repositorios correspondientes.
* Se deberá facilitar un video de máximo 5 minutos en donde se muestre la funcionalidad del laboratorio según la rúbrica de la sección anterior.
