# Pytest demo

Esta carpeta contiene un ejemplo de un paquete de Python `demo_api` y sus tests unitarios basados en `pytest`. Para una introducción sobre `pytest`, refiérase al [Pytest Index]( https://docs.pytest.org/en/7.1.x/index.html) y [Pytest Getting Started](https://docs.pytest.org/en/7.1.x/getting-started.html).

Además, este ejemplo utiliza [tox](https://tox.wiki/en/latest/index.html) para automatizar la administración del ambiente virtual para la construcción del paquete, sus dependencias y la ejecución de los tests. Para más información sobre `tox`, refiérase a [Tox basic usage](https://tox.wiki/en/latest/example/basic.html) y [Tox configuration specification](https://tox.wiki/en/latest/config.html).

## Pasos de construcción y ejecución de tests
Desde el contenedor de desarrollo en el directorio base del ejemplo (`examples/python/pytest_demo/`), ejecute el siguiente comando para preparar el ambiente virtual y ejecutar los tests:

```console
$ tox -e test
```

Note que esto genera una carpeta `.tox` en donde se almacenan los archivos temporales de `tox`, la cual puede eliminar para limpiar el ambiente.

Para reconstruir el ambiente y volver a preparar las dependencias puede ejecutar el siguiente comando:

```console
$ tox -re test
```

Para más detalles sobre el target de `test` y la configuración de `tox` del proyecto, refiérase el archivo `tox.ini`.

#### Parametrización de pytest con tox

Puede pasar argumentos extra de `pytest` por medio de `tox` de la siguiente manera:

```console
$ tox -e test -- <extra pytest args>
```

Por ejemplo, para correr `10` veces los tests con el prefijo `test_demo_api_mult` y utilizando una semilla aleatoria de `42`:

```console
$ tox -e test -- -k test_demo_api_mult --count=10 --randomly-seed=42
```

Para ver los diferentes argumentos que puede tomar `pytest` refiérase a [How to invoke pytest](https://docs.pytest.org/en/7.1.x/how-to/usage.html), así como correr el siguiente comando:

```console
$ tox -e test -- --help
```
