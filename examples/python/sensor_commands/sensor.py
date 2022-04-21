import json
import operator
import threading
import collections
from random import randint
from abc import ABC, abstractmethod
from typing import Optional, List, Dict, Callable, Deque

from command import Command, MessageCommand, CommandRunner

ThreshCompareStrategy = Callable[[float,float],bool]
ThreshHandleStrategy = Callable[[float,float],None]


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


class TempSensor(Sensor):
    def __init__(self, name: str) -> None:
        super().__init__(name, "temperature", "celsius")

    def read(self) -> float:
        return randint(0, 100)


class LevelSensor(Sensor):
    def __init__(self, name: str) -> None:
        super().__init__(name, "level", "meters")

    def read(self) -> float:
        return randint(1, 50)


class SensorFactory():
    def __init__(self) -> None:
        self._sensor_type_to_cls = {
            "temperature": TempSensor,
            "level": LevelSensor,
        }

    def __call__(self, name: str, sensor_type: str) -> Sensor:
        sensor_cls = self._sensor_type_to_cls[sensor_type]
        return sensor_cls(name)


class SensorAnalyzer(ABC):
    @abstractmethod
    def update(self, value: float):
        pass


class SensorAvgThreshAnalyzer(SensorAnalyzer):
    def __init__(
            self,
            avg_thresh: float, max_samples: int = 10,
            compare_fn: Optional[ThreshCompareStrategy] = None,
            handle_fn: Optional[ThreshHandleStrategy] = None
    ) -> None:
        self.samples: Deque[float] = collections.deque()
        self.avg_thresh = avg_thresh
        self.max_samples = max_samples
        self.avg_current: float = 0.0
        self.compare_fn = compare_fn
        self.handle_fn = handle_fn

    def set_compare_strategy(self, func) -> None:
        self.compare_fn = func

    def set_handle_strategy(self, func) -> None:
        self.handle_fn = func

    def _update_avg(self, value: float) -> bool:
        full = len(self.samples) >= self.max_samples
        if not full:
            self.samples.append(value)
        else:
            self.samples.popleft()
            self.samples.append(value)
            self.avg_current = sum(self.samples) / len(self.samples)
        return full

    def update(self, value: float):
        assert self.compare_fn is not None, "Threshold compare strategy is not set"
        assert self.handle_fn is not None, "Threshold handle strategy is not set"

        full = self._update_avg(value)
        if not full:
            return

        do_handle = self.compare_fn(self.avg_current, self.avg_thresh)
        if do_handle:
            self.handle_fn(self.avg_current, self.avg_thresh)
            self.samples.clear()
            self.avg_current = 0.0


def set_avg_thresh_alert_handle_strategy(
        analyzer: SensorAvgThreshAnalyzer,
        cmd_runner: CommandRunner):

    def send_alert_cmd(avg_current: float, avg_thresh: float):
        msg = f"Alert: average {avg_current} is above threshold {avg_thresh}!"
        cmd = MessageCommand(msg)
        cmd_runner.send(cmd)
    analyzer.set_handle_strategy(send_alert_cmd)


def set_above_avg_thresh_compare_strategy(
        analyzer: SensorAvgThreshAnalyzer):
    analyzer.set_compare_strategy(operator.gt)


def set_below_avg_thresh_compare_strategy(
        analyzer: SensorAvgThreshAnalyzer):
    analyzer.set_compare_strategy(operator.lt)


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
