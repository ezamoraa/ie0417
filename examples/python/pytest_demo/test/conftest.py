from pytest import fixture
import logging

# NOTE: The fixtures here will be available for all tests.
# For more information about fixtures and conftest.py refer to:
# https://docs.pytest.org/en/7.1.x/how-to/fixtures.html


@fixture(scope='module')
def example_mod_fixt():
    logging.info("This is a module-scope example fixture!")
    return [1, 2, 3]


@fixture(scope='function')
def example_func_fixt():
    logging.info("This is a function-scope example fixture!")
    data = {
        "name": "Data",
        "list": ["a", "b", "c"],
    }
    return data
