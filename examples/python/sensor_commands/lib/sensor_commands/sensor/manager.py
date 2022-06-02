import json
from typing import Optional, List, Dict, Type, Any

from ..command import Command
from .sensors import SensorFactory
from .sensor import Sensor, SensorReadCommand, SensorAnalyzer


class SensorManager:
    """
    Manager class to control sensor objects lifecycle.

    :param str config_filename: Name of the file with the sensors config.
    """
    def __init__(self, config_filename: str = "") -> None:
        self.config_filename = config_filename
        self.sensor_factory = SensorFactory()
        self.sensors: Dict[str, Sensor] = {}
        self.sensors_per_type: Dict[str, Dict[str, Sensor]] = {}
        if self.config_filename:
            self._init_config()

    def _init_config(self) -> None:
        """
        Initializes the manager from a configuration file.
        """
        # Parse config file
        with open(self.config_filename) as config_file:
            config_info = json.load(config_file)
            sensors_info = config_info["sensors"]
            # Create sensors
            for sensor_info in sensors_info:
                name = sensor_info["name"]
                stype = sensor_info["type"]
                self.create_sensor(name, stype)

    def _register_sensor(self, sensor: Sensor) -> None:
        """
        Registers a sensor in the manager dictionaries.

        :param sensor: Sensor instance.
        :type sensor: :class:`Sensor`
        """
        name = sensor.name()
        stype = sensor.type()
        self.sensors[name] = sensor
        if stype not in self.sensors_per_type:
            self.sensors_per_type[stype] = {}
        self.sensors_per_type[stype][name] = sensor

    def _unregister_sensor(self, sensor: Sensor) -> None:
        """
        Unregisters a sensor from the manager dictionaries.

        :param sensor: Sensor instance.
        :type sensor: :class:`Sensor`
        """
        name = sensor.name()
        stype = sensor.type()
        del self.sensors_per_type[stype][name]
        if not self.sensors_per_type[stype]:
            del self.sensors_per_type[stype]
        del self.sensors[name]

    def create_sensor(self, name: str, stype: str) -> None:
        """
        Creates and registers a sensor.

        :param name: Sensor name.
        :param stype: Sensor type.
        """
        assert name not in self.sensors, (
            f"Sensor with name {name} already exists"
        )
        sensor = self.sensor_factory(name, stype)
        self._register_sensor(sensor)

    def destroy_sensor(self, name: str) -> None:
        """
        Destroys a sensor by unregistering it.

        :param name: Sensor name.
        """
        assert name in self.sensors, (
            f"Sensor with name {name} does not exist"
        )
        sensor = self.sensors[name]
        self._unregister_sensor(sensor)

    def register_sensor_type(self, stype: str, cls: Type[Sensor]):
        """
        Registers a new sensor type in the factory.

        :param stype: Sensor type.
        :param cls: Sensor class.
        """
        self.sensor_factory.register_type_cls(stype, cls)

    def unregister_sensor_type(self, stype: str):
        """
        Unregisters a sensor type from the factory.

        :param stype: Sensor type.
        """
        self.sensor_factory.unregister_type_cls(stype)

    def get_supported_sensor_types(self) -> List[str]:
        """
        Returns the list of supported sensor types from the factory.
        """
        return self.sensor_factory.supported_types

    def get_sensors_info(self) -> Dict[str, Dict[str, Any]]:
        """
        Returns a dictionary with the sensors information.
        """
        return {
            sensor.name(): {
                "type": sensor.type()
            }
            for sensor in self.sensors.values()
        }

    def get_sensor_names_per_type(self, stype: str) -> List[str]:
        """
        Returns the list of sensor names for a sensor type.

        :param stype: Sensor type.
        """
        names: List[str] = []
        type_sensors = self.sensors_per_type.get(stype)
        if type_sensors is not None:
            names = [name for name in type_sensors.keys()]
        return names

    def create_sensor_read_cmd(
            self,
            sensor_name: str,
            analyzer: Optional[SensorAnalyzer] = None
    ) -> Command:
        """
        Creates a command to read a sensor.

        :param str sensor_name: Name of the sensor to read.
        :param sensor_name: Name of the sensor to read.
        :param analyzer: Sensor analyzer to send the readings.
        :type analyzer: :class:`SensorAnalyzer`
        """
        sensor = self.sensors[sensor_name]
        return SensorReadCommand(sensor, analyzer)
