"""
sensors module entry point.
"""
from ..sensor import Sensor

from .temp import TempSensor
from .level import LevelSensor


class SensorFactory():
    """
    Factory that creates a sensor of a given type name.
    """
    def __init__(self) -> None:
        # Dictionary that maps names of sensor types to classes.
        self._sensor_type_to_cls = {
            "temperature": TempSensor,
            "level": LevelSensor,
        }

    @property
    def supported_types(self):
        """
        Returns the list of names for the supported sensor types.
        """
        return [stype for stype in self.sensor_type_to_cls.keys()]

    def __call__(self, name: str, stype: str) -> Sensor:
        """
        Creates the sensor.

        :param str name: Name of the sensor to create.
        :param str stype: Sensor type.
        """
        sensor_cls = self._sensor_type_to_cls[stype]
        return sensor_cls(name)
