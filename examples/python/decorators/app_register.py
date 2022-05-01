"""
Application register decorator example module.

This module exercises the concept of decorators to register functions into
a global application object.
"""

from typing import Callable


class App:
    """
    Application class that registers functions to a dictionary using
    decorators.
    """
    def __init__(self):
        self._funcs = {}

    def register(self, name: str):
        """
        Returns a decorator that registers the function with some name.

        :param str name: Name used to register the function.
        """
        def wrapper(func: Callable):
            self._funcs[name] = func
            return func
        return wrapper

    def call_registered(self, name: str, *args, **kwargs):
        """
        Invokes a previously registered function according to name.

        :param str name: Name of the registered function.
        :param args: List of positional arguments.
        :param kwargs: List of keyword arguments.
        """
        return self._funcs[name](*args, **kwargs)


app = App()


@app.register("test")
def test_func(*args, **kwargs):
    print(f"Test log! {args}, {kwargs}")

# This is equivalent to:
# app.register("test")(test_func)


def main():
    app.call_registered("test", 2, t=1)


if __name__ == "__main__":
    main()
