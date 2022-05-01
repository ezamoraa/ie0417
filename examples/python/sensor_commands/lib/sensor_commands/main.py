import logging
from random import choice

from . import command
from .args import parse_args
from .sensor.manager import SensorManager
from .sensor.analyzers import avg_thresh_analyzer as avt


log = logging.getLogger(__name__)


def main():
    """
    Sensor commands application main function.
    """
    args = parse_args()

    config_name = args.config
    sensor_type_name = args.sensor_type
    sensor_cmd_per_period, sensor_period_sec = (100, 5)
    alert_cmd_per_period, alert_period_sec = (2, 1)
    analyzer_avg_thresh = 10
    num_read_commands = 200

    # Set up command runners
    sensor_mgr = SensorManager(config_name)
    sensor_cmd_runner = command.CommandRunner(
        cmd_per_period=sensor_cmd_per_period,
        period_sec=sensor_period_sec)
    alert_cmd_runner = command.CommandRunner(
        cmd_per_period=alert_cmd_per_period,
        period_sec=alert_period_sec)
    sensor_cmd_runner.start()
    alert_cmd_runner.start()

    # Set up sensor analyzer with "above average threshold alert" strategy
    analyzer = avt.SensorAvgThreshAnalyzer(avg_thresh=analyzer_avg_thresh)
    avt.set_alert_handle_strategy(analyzer, alert_cmd_runner)
    avt.set_above_compare_strategy(analyzer)

    # Generate read commands for temp sensors
    temp_sensor_names = sensor_mgr.get_sensor_names_per_type(sensor_type_name)
    for _ in range(num_read_commands):
        rand_sensor_name = choice(temp_sensor_names)
        read_cmd = sensor_mgr.create_sensor_read_cmd(rand_sensor_name,
                                                     analyzer)
        sensor_cmd_runner.send(read_cmd)

    # Teardown command runners
    sensor_cmd_runner.stop()
    alert_cmd_runner.stop()


if __name__ == "__main__":
    main()
