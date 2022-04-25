from ..sensor import Sensor

from .temp import TempSensor
from .level import LevelSensor


class SensorFactory():
    def __init__(self) -> None:
        self._sensor_type_to_cls = {
            "temperature": TempSensor,
            "level": LevelSensor,
        }

    def __call__(self, name: str, sensor_type: str) -> Sensor:
        sensor_cls = self._sensor_type_to_cls[sensor_type]
        return sensor_cls(name)
