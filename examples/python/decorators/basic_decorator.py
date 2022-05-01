"""
Basic decorator example module.

This module exercises the concept of decorators to wrap around functions.
"""

from typing import Callable


def basic_decorator(func: Callable):
    """
    Basic decorator that wraps around a function to print some logs.

    :param func func: Function to decorate.
    """
    def wrapper(*args, **kwargs):
        print(f"Before running {func.__name__}")
        ret = func(*args, **kwargs)
        print(f"After running {func.__name__}")
        return ret
    return wrapper


@basic_decorator
def mult_sum(a, b, c):
    """
    Multiply and sum operation.

    :param a: First multiply operator
    :param b: Second multiply operator
    :param c: Sum operator
    """
    return a * b + c


def main():
    r = mult_sum(10, 20, 10)
    print(f"Result is {r}")


if __name__ == "__main__":
    main()
