"""
sensors module entry point.
"""
from typing import List, Type
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
    def supported_types(self) -> List[str]:
        """
        Returns the list of names for the supported sensor types.
        """
        return [stype for stype in self._sensor_type_to_cls.keys()]

    def register_type_cls(self, stype: str, cls: Type[Sensor]):
        """
        Register a new sensor type in the factory.

        :param str stype: Sensor type.
        :param cls: Sensor class.
        """
        assert stype not in self._sensor_type_to_cls, (
            "Sensor type already registered"
        )
        self._sensor_type_to_cls[stype] = cls

    def unregister_type_cls(self, stype: str):
        """
        Unregisters a sensor type from the factory.

        :param str stype: Sensor type.
        """
        assert stype in self._sensor_type_to_cls, (
            "Sensor type not registered"
        )
        del self._sensor_type_to_cls[stype]

    def __call__(self, name: str, stype: str) -> Sensor:
        """
        Creates the sensor.

        :param str name: Name of the sensor to create.
        :param str stype: Sensor type.
        """
        sensor_cls = self._sensor_type_to_cls[stype]
        return sensor_cls(name)
