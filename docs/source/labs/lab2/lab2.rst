*************************************************
Laboratorio 2: Desarrollo de proyectos en Python
*************************************************

Requisitos previos
==================
* Siga los requisitos previos de `Laboratorio 1 <../lab1/lab1.html>`_ para preparar el ambiente para este laboratorio.

Introducción
============

El objetivo de este laboratorio consiste en implementar un prototipo sobre una parte de ``eieManager``, tal como se presentó en el problema de diseño del laboratorio 1. Recordando el bosquejo inicial del sistema:

.. image:: img/eie_manager.png
   :align: center

Para desbloquear al equipo que implementa ``Client`` lo antes posible, su equipo en ``eieLabs`` deberá implementar la funcionalidad básica del ``API`` para ``Client``.

Consideraciones
---------------

Tal que se cumplan los objetivos de negocio, se decidió que el ``API`` se expondrá mediante el protocolo ``HTTP``. Esto a su vez permite que se pueda interactuar fácilmente con un cliente implementado en cualquier lenguaje de programación.

Por otra parte, su equipo decidió que ``eieManager`` será codificado utilizando Python. Se concluyó que este lenguaje facilitaría cumplir con requerimientos no funcionales de modificabilidad y flexibilidad, tal que se responda rápidamente ante cambios en los features requeridos.

Sin embargo, para mantener un estándar de calidad de código y minimizar los errores de ejecución relacionados al tipado dinámico de Python, se utilizarán herramientas como `flake8 <https://flake8.pycqa.org/en/latest/>`_ y `mypy <https://mypy.readthedocs.io/en/stable/>`_ para validar el código. Además, se documentarán detalladamente las clases y funciones utilizando `docstrings <https://sphinx-rtd-tutorial.readthedocs.io/en/latest/docstrings.html>`_

Se decidió utilizar una arquitectura tipo `REST <https://www.ibm.com/cloud/learn/rest-apis>`_, con el fin de restringir el ``HTTP API`` para hacerlo sencillo y confiable. En particular, esta arquitectura se alinea a la necesidad de administrar los ``Devices`` como recursos que soportan operaciones de ``Create``, ``Read``, ``Update`` y ``Delete`` (`CRUD <https://en.wikipedia.org/wiki/Create,_read,_update_and_delete>`_). Más específicamente, se requiere que el prototipo soporte las siguientes operaciones:

* Crear y registrar un nuevo ``Device``. El dispositivo debe tener un identificador o nombre tipo string.

* Actualizar la información de un ``Device`` específico previamente registrado.

* Obtener la información de todos los ``Device`` registrados.

* Obtener la información de un ``Device`` específico previamente registrado.

* Eliminar un ``Device`` específico previamente registrado.

* Enviar un comando de administración a un ``Device`` específico y obtener su respuesta. La información requerida para ejecutar el comando debe pasarse por medio del ``Body`` del API request y no como parte del ``URL``.

Dentro de la información que debe estar asociada a un objeto ``Device`` genérico, se encuentran al menos los siguientes atributos:

* Nombre identificador único del dispositivo.

* Nombre del tipo del dispositivo. Este tipo será asociado a subclases de ``Device``.

* Lista de nombres de comandos soportados.

* Información de conexión (host/IP y puerto). Esto debe ser un solo string de la forma ``host:port``. Por ejemplo ``172.17.0.3:7777``.

Para facilitar la modificabilidad del ``eieManager`` y según el principio de diseño *Open for Extension, Closed for Modification (OCP)*, se utilizarán clases abstractas que permitirán manejar a cualquier nuevo tipo de ``Device`` de manera genérica.

A pesar de que el ``APIServer`` permitirá crear y destruir dispositivos dinámicamente, el prototipo deberá inicializarse con una configuración predefinida de dispositivos. Esta configuración se realizará con un archivo tipo ``JSON``, y consistirá en una lista de diccionarios que tienen la información necesaria para poder crear las instancias iniciales.

Para aislar los cambios del administrador al soportar nuevos tipos de ``Device``, se utilizará únicamente el patrón de ``Factory`` para crear instancias específicas de ``Device``. El componente encargado de administrar los dispositivos se llamará ``DeviceManager``. Este debe encargarse de:

* Inicializar el registro de los ``Device`` a partir del archivo de configuración y un ``DeviceFactory``.

* Crear y registrar nuevos ``Device`` a partir de un diccionario de información y un ``DeviceFactory``.

* Destruir las instancias de ``Device`` ya registradas a partir de su identificador.

* Enviar un comando a un ``Device`` específico a partir de su identificador y recibir su respuesta.

Se sugiere revisar el ejemplo de ``sensor_commands`` para basar la implementación de ``DeviceManager`` y ``DeviceFactory``.

Para mantener el *Single Responsibility Principle (SRP)*, ``DeviceManager`` debe estar desacoplado del ``APIServer`` que implementa la lógica del ``REST API``. Sin embargo, es natural que el ``APIServer`` utilice los servicios de ``DeviceManager`` para administrar los ``Devices`` y procesar sus comandos.

Finalmente, con el objetivo de limitar la carga del servidor del API al procesar comandos, se utilizarán las clases ``CommandRunner`` y ``Command`` del ejemplo ``sensor_commands``. Estas clases pueden ser copiadas e integradas manualmente en su proyecto. Más específicamente, desde ``APIServer`` el ``DeviceManager`` debe recibir la información necesaria para ejecutar el comando y empaquetarla en objetos ``DeviceCommand`` que son enviados a un ``CommandRunner`` interno.

Note que estos comandos son procesados asincrónicamente desde otro thread (ver ``CommandRunner``), por lo que deberá utilizar `threading events <https://docs.python.org/3/library/threading.html#threading.Event>`_ para sincronizar y esperar por la respuesta del comando. Toda esta lógica de enviar el comando y esperar la respuesta puede implementarse internamente en un método de ``DeviceManager``.

Instrucciones
=============
A continuación se describirán las instrucciones de los apartados de este laboratorio.

Diseño
------

* Documente el diseño de los *endpoints* del ``REST API`` usando `Read The Docs`, tal como en el laboratorio 1.

* De ser necesario, utilice esta misma página para documentar decisiones particulares del diseño de componentes para lograr prototipo.

Implementación
--------------

* Cree un *import package* de Python que contenga una jerarquía de módulos para implementar el prototipo del API. Los componentes deben estar distribuidos lo más posible en diferentes archivos para facilitar la mantenibilidad del código. Para esto intente activamente disminuir las interdependecias entre componentes siguiendo los principios de diseño SOLID. La distribución específica de archivos queda a discreción, sin embargo, un ejemplo de como hacerlo puede ser una estrategia similar al ejemplo de ``sensor_commands``. Utilice `virtual environments <https://docs.python.org/3/tutorial/venv.html>`_ de Python para desarrollar los paquetes.

* Para la implementación del prototipo de ``eieManager`` y su componente ``APIServer``, utilice el framework de `FastAPI <https://fastapi.tiangolo.com/it/tutorial/>`_ + `Uvicorn <https://www.uvicorn.org/>`_. En particular, para poder crear el *import package* tal como se solicita refiérase al siguiente tutorial de `FastAPI Bigger Applications <https://fastapi.tiangolo.com/it/tutorial/bigger-applications/>`_. Para el diseño del ``REST API`` puede referirse a los ejemplos del repositorio del curso y al `tutorial de Web APIs en Python <https://realpython.com/api-integration-in-python/>`_. Se sugiere utilizar `requests` de Python para probar el ``REST API``, tal como se muestra en los ejemplos del repositorio del curso.

* Implemente el módulo de ``DeviceManager`` y sus dependencias e integrelo con la capa del ``APIServer``. Ya que esto es un prototipo, para la ejecución de los comandos solo imprima la información del mismo, siguiendo el formato propuesto en el laboratorio 1: ``(command, device, arg1=val1, ..., argN=valN)``. Para esto, agregue una lista de nombres de comandos soportados de prueba pero no implemente lógica específica para cada uno de ellos (además del print).

* Implemente la lógica para empaquetar los requests de comandos en objetos ``DeviceCommand`` y para enviarlos al ``CommandRunner``, así como sincronizarlo esperando la respuesta con `threading events`. Para entregar la respuesta puede escribirla en algún atributo del objeto comando. Esta respuesta puede ser un string arbitrario para efectos demostrativos.

* Cree un *distribution package* de Python para la aplicación del ``eieManager`` utilizando ``setuptools`` (``setup.py``). Asegúrese de incluir en un archivo ``requirements.txt`` todas las dependencias de paquetes de Python que requiera el proyecto. Para esta parte puede basarse una vez más en el ejemplo de ``sensor_commands``.

Calidad del código
------------------
* Documente todas las clases y funciones del prototipo siguendo el formato ``docstrings``. Refiérase a los ejemplos del repositorio y al link de ``docstrings`` presentado en la sección de consideraciones.

* Asegúrese de que la implementación final del prototipo pase todas las pruebas de estilo de código de ``flake8``.

* Asegúrese de que la implementación final del prototipo pase todas las pruebas de validación de tipos de ``mypy``.

.. tip::

   Pueden haber excepciones en donde se debe hacer skip de errores de ``mypy`` al utilizar paquetes externos.

Publicación
-----------

* Publique el paquete del prototipo al *Testing Python Package Index (TestPyPI)*, siguiendo las instrucciones del tutorial de `packaging projects <https://packaging.python.org/en/latest/tutorials/packaging-projects/#uploading-the-distribution-archives>`_. El nombre del paquete queda a discreción pero se sugiere ``eie-manager``.

Evaluación
==========
Este laboratorio se realizará preferiblemente en grupos de 2 o 3 personas.

Rúbrica
-------

+--------------------+------------------------------------------------------------------+------------+
| Apartado           |  Criterios de evaluación                                         | Porcentaje |
+====================+==================================================================+============+
| Diseño             || Documentar en Read The Docs la descripción detallada de los     | 10%        |
|                    || endpoints REST y cualquier otra particularidad del diseño del   |            |
|                    || prototipo que se considere relevante.                           |            |
+--------------------+------------------------------------------------------------------+------------+
| Implementación     || Lograr implementar la funcionalidad del REST API utilizando     | 70%        |
|                    || FastAPI y Uvicorn. El DeviceManager debe estar correctamente    |            |
|                    || integrado con la capa del API para responder a los requests     |            |
|                    || de administración de devices y de procesamiento de comandos.    |            |
|                    || Además, el CommandRunner debe ser utilizado para limitar la     |            |
|                    || cantidad de comandos ejecutados por unidad de tiempo.           |            |
|                    || Los threading events son utilizados para sincronizar el thread  |            |
|                    || que procesa el request con el del CommandRunner.                |            |
|                    || Un video de máximo 5 minutos demuestra la funcionalidad del     |            |
|                    || REST API procesando requests de administracion y comandos       |            |
|                    || de dispositivos con algún cliente (requests, curl, etc).        |            |
+--------------------+------------------------------------------------------------------+------------+
| Calidad del código || Las pruebas de flake8 y mypy ejecutados desde la raiz del       | 10%        |
|                    || paquete de Python pasan satisfactoriamente, sin error.          |            |
+--------------------+------------------------------------------------------------------+------------+
| Publicación        || El paquete de distribución del prototipo debe estar disponible  | 10%        |
|                    || en internet y poder descargarse del repositorio de TestPyPI.    |            |
+--------------------+------------------------------------------------------------------+------------+

Revisión
--------

* Se debe demostrar con base en los autores de los commits de Git que todos los miembros del grupo trabajaron colaborativamente en el laboratorio utilizando control de versiones. De no ser así, no se asignará puntaje a los miembros que no contribuyeron (a menos que se justifique una excepción).
* Se utilizará la fecha del último commit de `merge` de un Pull Request (PR) de GitHub que modifique la documentación del laboratorio respectivo para determinar si la entrega se realizó a tiempo.
* Si se realizan entregas tardías, se rebajará 10% más por cada día extra (-10% primer día, -20% segundo día, etc).
* Se revisará la página de documentación de `Read the Docs` indicada en la tarea de Mediación Virtual del laboratorio.
* Para los grupos, sólo es necesario subir los cambios en el repositorio de uno de los miembros. Los demás miembros pueden hacer referencia a dicho repositorio y/o sincronizar los cambios en sus repositorios correspondientes.
* Se deberá facilitar un video de máximo 5 minutos en donde se muestre la funcionalidad del laboratorio corriendo en un ambiente virtual de Python.
