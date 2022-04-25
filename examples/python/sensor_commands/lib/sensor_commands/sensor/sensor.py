import json
from abc import ABC, abstractmethod
from typing import Optional, List, Dict

from ..command import Command


class Sensor(ABC):
    def __init__(self, name: str, stype: str, unit: str) -> None:
        self._name = name
        self._type = stype
        self._unit = unit

    def name(self) -> str:
        return self._name

    def type(self) -> str:
        return self._type

    def unit(self) -> str:
        return self._unit

    @abstractmethod
    def read(self) -> float:
        pass


class SensorAnalyzer(ABC):
    @abstractmethod
    def update(self, value: float):
        pass


class SensorReadCommand(Command):
    def __init__(self, sensor: Sensor, analyzer: Optional[SensorAnalyzer] = None) -> None:
        self.sensor = sensor
        self.analyzer = analyzer

    def execute(self) -> None:
        name = self.sensor.name()
        stype = self.sensor.type()
        unit = self.sensor.unit()
        value = self.sensor.read()
        if self.analyzer is not None:
            self.analyzer.update(value)
        print(f"SensorReadCommand: [{stype}] {name}: {value} {unit}")
