"""
Higher-order functions example module.

This module exercises the concept of higher-order functions that
take other functions as parameters.
"""

import operator
from typing import Callable


def op_higher_order_func(op_func: Callable, a, b, c=1):
    """
    Higher order function that applies a custom operator.

    :param func op_func: Operator function that takes two arguments.
    :param a: Operator A.
    :param b: Operator B.
    :param c: Multiply factor.
    """
    return c * op_func(a, b)


def main():
    r = op_higher_order_func(operator.add, 1, 2)
    print(f"Result 1: {r}")

    r = op_higher_order_func(operator.sub, 1, 2, 4)
    print(f"Result 2: {r}")


if __name__ == "__main__":
    main()
