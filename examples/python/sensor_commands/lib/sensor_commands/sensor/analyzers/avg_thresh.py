import operator
import collections
from typing import Optional, Callable, Deque

from ..sensor import SensorAnalyzer
from ...command import CommandRunner, MessageCommand

ThreshCompareStrategy = Callable[[float,float],bool]
ThreshHandleStrategy = Callable[[float,float],None]


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


def set_alert_handle_strategy(analyzer: SensorAvgThreshAnalyzer,
                              cmd_runner: CommandRunner):
    def send_alert_cmd(avg_current: float, avg_thresh: float):
        msg = f"Alert: average {avg_current} is above threshold {avg_thresh}!"
        cmd = MessageCommand(msg)
        cmd_runner.send(cmd)
    analyzer.set_handle_strategy(send_alert_cmd)


def set_above_compare_strategy(analyzer: SensorAvgThreshAnalyzer):
    analyzer.set_compare_strategy(operator.gt)


def set_below_compare_strategy(analyzer: SensorAvgThreshAnalyzer):
    analyzer.set_compare_strategy(operator.lt)
