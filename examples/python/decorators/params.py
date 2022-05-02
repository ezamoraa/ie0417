"""
Decorator with parameters example module.

This module exercises the concept of decorators that have custom parameters.
"""

import functools
from typing import Callable


def do_repeated(n: int = 1):
    """
    Decorator that repeats the function multiple times.

    This is helpful to demonstrate the concept of decorators with parms.
    We can think of this function as a closure that attaches the parameters
    and returns a decorator without parameters.

    :param int n: Message to attach to print function.
    """
    def decorator(func: Callable):
        @functools.wraps(func)
        def repeated_wrapper(*args, **kwargs):
            for _ in range(n):
                func(*args, **kwargs)
        return repeated_wrapper
    return decorator


def log_msg(msg: str):
    print(f"Logging message: {msg}")


@do_repeated(5)
def log_msg_repeated(msg: str):
    print(f"Logging message: {msg}")


def main():
    print("Original function:")
    log_msg("Test original!")

    print("Decorated manually:")
    log_msg_rep_manual = do_repeated(10)(log_msg)
    log_msg_rep_manual("Test decorated manually!")

    print("Decorated using syntax sugar:")
    log_msg_repeated("Test decorated syntax!")


if __name__ == "__main__":
    main()
