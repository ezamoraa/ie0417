"""
Basic closure example module.

This module exercises the concept of closures to attach data to functions.
"""

import functools


def print_msg_closure(msg: str):
    """
    Closure function that returns a function that prints a message.

    :param str msg: Message to attach to print function.
    """
    def printer():
        print(msg)
    return printer


def main():
    printer_1 = print_msg_closure("Hello msg closure!")
    for _ in range(5):
        printer_1()

    printer_2 = functools.partial(print, "Hello msg partial!")
    for _ in range(5):
        printer_2()


if __name__ == "__main__":
    main()
