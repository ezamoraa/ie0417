import json
from typing import Optional, List, Dict

from ..command import Command
from .sensors import SensorFactory
from .sensor import Sensor, SensorReadCommand, SensorAnalyzer


class SensorManager:
    def __init__(self, config_filename: str) -> None:
        self.config_filename = config_filename
        self.sensor_factory = SensorFactory()
        self.sensors: Dict[str,Sensor] = {}
        self.sensors_per_type: Dict[str,Dict[str,Sensor]] = {}
        self.init_config()

    def _init_sensors_per_type(self):
        for sensor in self.sensors.values():
            stype = sensor.type()
            name = sensor.name()
            if stype not in self.sensors_per_type:
                self.sensors_per_type[stype] = {}
            self.sensors_per_type[stype][name] = sensor

    def init_config(self) -> None:
        # Parse config file
        with open(self.config_filename) as config_file:
            config_info = json.load(config_file)
            sensors_info = config_info["sensors"]
            # Create sensors
            for sensor_info in sensors_info:
                name = sensor_info["name"]
                stype = sensor_info["type"]
                self.sensors[name] = self.sensor_factory(name, stype)

        self._init_sensors_per_type()

    def get_sensor_names(self) -> List[str]:
        return [name for name in self.sensors.keys()]

    def get_sensor_names_per_type(self, stype: str) -> List[str]:
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
        sensor = self.sensors[sensor_name]
        return SensorReadCommand(sensor, analyzer)
