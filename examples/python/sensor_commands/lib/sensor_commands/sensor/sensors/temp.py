from random import randint

from ..sensor import Sensor


class TempSensor(Sensor):
    def __init__(self, name: str) -> None:
        super().__init__(name, "temperature", "celsius")

    def read(self) -> float:
        return randint(0, 100)
