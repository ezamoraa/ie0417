"""
demo_api module entry point.
"""

__author__ = 'Esteban Zamora Alvarado'
__email__ = 'esteban.zamora.al@gmail.com'
__version__ = '0.0.1'


def add(a: int, b: int):
    return a + b


def mult(a: int, b: int):
    return a * b


def double_if_pos(a: int):
    assert a >= 0, "a is negative, cannot proceed"
    return 2 * a
