#!/usr/bin/env python3

import random


class RandomGenerator:
    def __init__(self, seed: int = 0, generator=random):
        self.gen = generator
        self.seed = seed

    def get_seed(self):
        """
        Gets the generator seed value

        :return: Seed value
        """
        return self.seed

    def get_bool(self, generator=random):
        """
        Generates a random bool value

        :param generator: random number generator
        :return: Boolean (True or False)
        """
        return bool(self.gen.getrandbits(1))

    def get_unique_int_list(self, low: int, high: int, num: int):
        """
        Generates a random list of unique integers

        :param generator: random number generator
        :return: List of random values
        """
        return self.gen.sample(range(low, high + 1), num)
