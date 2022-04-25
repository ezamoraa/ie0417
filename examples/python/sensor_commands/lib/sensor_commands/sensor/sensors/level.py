from random import randint

from ..sensor import Sensor


class LevelSensor(Sensor):
    def __init__(self, name: str) -> None:
        super().__init__(name, "level", "meters")

    def read(self) -> float:
        return randint(1, 50)
