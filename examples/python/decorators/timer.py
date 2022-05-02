"""
Timer decorator example module.

This module defines a decorator that can measure the execution time
of functions.
"""

import time
import functools
from typing import Callable


def timer_decorator(func: Callable):
    """
    Basic decorator that prints the execution elapsed time of a function.

    :param func func: Function to decorate.
    """
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        t1 = time.time()
        ret = func(*args, **kwargs)
        t2 = time.time()
        print(f'Function {func.__name__} elapsed time is {(t2-t1):.4f}s')
        return ret
    return wrapper


@timer_decorator
def busy_loop(n):
    """
    Dummy function that performs a busy loop.
    """
    for i in range(n):
        for j in range(100000):
            i*j


def main():
    print(f"Function name is {busy_loop.__name__}")
    busy_loop(10)


if __name__ == "__main__":
    main()
