import operator
import collections
from typing import Optional, Callable, Deque

from ..sensor import SensorAnalyzer
from ...command import CommandRunner, MessageCommand

ThreshCompareStrategy = Callable[[float, float], bool]
ThreshHandleStrategy = Callable[[float, float], None]


class SensorAvgThreshAnalyzer(SensorAnalyzer):
    """
    Sensor analyzer that triggers an action according to a running average
    threshold.

    :param float avg_thresh: Threshold value for running average.
    :param int max_samples: Number of samples to consider in running average.
    :param func compare_fn: Strategy function to compare running average with
                            threshold.
    :param func handle_fn: Strategy function to define trigger action.
    """
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

    def set_compare_strategy(self, function: ThreshCompareStrategy) -> None:
        """
        Sets compare strategy function.

        The comparison is performed as compare_fn(avg_current, avg_threshold).
        This should return a boolean. If True, the handle action is executed.

        :param func function: Strategy function to set.
        """
        self.compare_fn = function

    def set_handle_strategy(self, function: ThreshHandleStrategy) -> None:
        """
        Sets handle strategy function.

        :param func function: Strategy function to set.
        """
        self.handle_fn = function

    def _update_avg(self, value: float) -> bool:
        """
        Update running average with new sample value.

        :param float value: Sample value.
        :return: Boolean that indicates if the sample queue has reached
                 its full capacity.
        """
        full = len(self.samples) >= self.max_samples
        if not full:
            self.samples.append(value)
        else:
            self.samples.popleft()
            self.samples.append(value)
            self.avg_current = sum(self.samples) / len(self.samples)
        return full

    def update(self, value: float):
        """
        Updates the analyzer with a new sample value.

        :param float value: Sample value.
        """

        # Make sure that strategies are set
        assert self.compare_fn is not None, (
         "Threshold compare strategy is not set"
        )
        assert self.handle_fn is not None, (
         "Threshold handle strategy is not set"
        )

        full = self._update_avg(value)
        if not full:
            # We don't set the average until we get to full capacity
            return

        do_handle = self.compare_fn(self.avg_current, self.avg_thresh)
        if do_handle:
            # The comparison triggered the handler
            self.handle_fn(self.avg_current, self.avg_thresh)
            self.samples.clear()
            self.avg_current = 0.0


def set_alert_handle_strategy(analyzer: SensorAvgThreshAnalyzer,
                              cmd_runner: CommandRunner):
    """
    Sets the analyzer's handle strategy as to send an alert
    command to a command runner.

    :param analyzer: Sensor analyzer to set the strategy to.
    :param cmd_runner: Command runner to send the alert commands to.
    """
    def send_alert_cmd(avg_current: float, avg_thresh: float):
        msg = f"Alert: average {avg_current} is above threshold {avg_thresh}!"
        cmd = MessageCommand(msg)
        cmd_runner.send(cmd)
    analyzer.set_handle_strategy(send_alert_cmd)


def set_above_compare_strategy(analyzer: SensorAvgThreshAnalyzer):
    """
    Sets the analyzer's compare strategy as a "greater than" comparison.

    :param analyzer: Sensor analyzer to set the strategy to.
    """
    analyzer.set_compare_strategy(operator.gt)


def set_below_compare_strategy(analyzer: SensorAvgThreshAnalyzer):
    """
    Sets the analyzer's compare strategy as a "lower than" comparison.

    :param analyzer: Sensor analyzer to set the strategy to.
    """
    analyzer.set_compare_strategy(operator.lt)
