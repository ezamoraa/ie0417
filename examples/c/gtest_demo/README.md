# Gtest demo (C/C++)

Esta carpeta contiene un ejemplo de una biblioteca `demo_api` y sus tests unitarios basados en `googletest` (`gtest`). Para una introducción sobre `googletest`, refiérase al [Googletest Primer](https://google.github.io/googletest/primer.html).

## Pasos de construcción
Desde el contenedor de desarrollo en el directorio base del ejemplo (`examples/c/gtest_demo/`) ejecute los siguiente comandos:

```console
$ mkdir build
$ cd build
$ cmake ..
$ make
```

Note que este comando `make` default no construye los test unitarios.

## Tests unitarios

Los componentes relacionados a los tests unitarios se encuentra en la carpeta `test`. El framework de `googletest` se descarga al configurarse el proyecto con CMake, el cual se enlaza al ejecutable `unittest`.

Los programas de tests se encuentran en la carpeta `test/tests`. Además, se creó una biblioteca de ejemplo con clases utilitarias de testing en `test/testutil`.

Para construir los tests unitarios puede correr el siguiente comando desde la carpeta build de CMake:

```console
$ make unittest
```

Para ejecutar los tests unitarios existen dos opciones.

### Ejecución de tests con CTest

[CTest](https://cmake.org/cmake/help/latest/manual/ctest.1.html) es la herramienta genérica de CMake para registrar y ejecutar tests de diversos tipos.

Los ejecutables de los tests se registran utilizando funciones de CMake como `add_test`. En particular, para tests de `gtest` existe un [módulo de CMake](https://cmake.org/cmake/help/latest/module/GoogleTest.html) con funciones que facilitan este proceso, tales como `gtest_discover_tests`.

Para lanzar todos los tests registrados con `CTest` puede ejecutar el siguiente comando desde la carpeta build de CMake:

```console
$ make test
```

De forma similar, puede ejecutar directamente `CTest`:

```console
$ ctest
```

Esta última alternativa permite pasar argumentos a `ctest` para filtrar los tests, entre otros. Para más información refiérase a la documentación de `CTest`.

Al ser `CTest` una herramienta genérica existen limitaciones sobre la parametrización de los ejecutables de testing, por lo que no es sencillo pasar argumentos de `gtest` en tiempo de ejecución. Por esta razón, se crea un custom target para ejecutar los tests directamente.

### Ejecución de tests con run_unittest (custom target)

Para lanzar todos los tests en el ejecutable de `unittest` puede correr el siguiente comando:

```console
$ make run_unittest
```

Para pasar argumentos extra al lanzar el ejecutable de `unittest` puede utilizar la variable `GTEST_ARGS`:

```console
$ make run_unittest GTEST_ARGS="--gtest_filter=demo_api_fixture.* --gtest_random_seed=10 --gtest_repeat=5"
```

Para ver los diferentes argumentos que puede tomar el ejecutable de `unittest` puede correr el siguiente comando:

```console
$ make run_unittest GTEST_ARGS="--help"
```
