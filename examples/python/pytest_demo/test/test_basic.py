import logging
import pytest

import demo_api


def test_demo_api_add_op():
    r = demo_api.add(5, 9)
    logging.info(f"Add result: {r}")
    assert r == 14, "Addition failed!"


def test_demo_api_mult_op():
    r = demo_api.mult(5, 4)
    logging.info(f"Mult result: {r}")
    assert r == 20, "Multiplication failed!"


def test_demo_api_double_if_pos_op():
    r = demo_api.double_if_pos(5)
    logging.info(f"Double result: {r}")
    assert r == 10, "Double failed!"


def test_demo_api_double_if_pos_sanity():
    with pytest.raises(AssertionError):
        demo_api.double_if_pos(-4)
