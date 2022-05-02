import threading
import queue
from abc import ABC, abstractmethod
from ratelimiter import RateLimiter  # type: ignore
from typing import Optional


class Command(ABC):
    """
    Generic command representation.
    """
    @abstractmethod
    def execute(self) -> None:
        """
        Executes the command logic.
        """
        pass


class MessageCommand(Command):
    """
    Command that prints a custom message.

    :param str msg: Message to be printed.
    """
    def __init__(self, msg: str) -> None:
        self.msg = msg

    def execute(self) -> None:
        """
        Prints the command message.
        """
        print(f"MessageCommand: {self.msg}")


class CommandRunner:
    """
    Executes commands in a thread using a rate limiter.

    :param int cmd_per_period: Max mumber of commands to run per time period.
    :param int period_sec: Number of seconds per time period.
    :param int cmd_qsize: Size of the queue to store commands to be processed.
    """
    def __init__(
            self,
            cmd_per_period: int = 10,
            period_sec: int = 1,
            cmd_qsize: int = 100,
    ) -> None:
        self.cmd_per_period = cmd_per_period
        self.period_sec = period_sec
        self.cmd_rate_limiter = RateLimiter(
            max_calls=self.cmd_per_period, period=self.period_sec)
        self.cmd_queue: queue.Queue[Optional[Command]] = queue.Queue()
        self.cmd_worker = threading.Thread(target=self.run)

    def send(self, cmd: Command) -> bool:
        """
        Queues command for execution (non-blocking).

        :param cmd: Command object to queue.
        :type cmd: :class:`Command`
        :return: A boolean indicating if the command was queued successfully.
                 If False, this means that the queue was full.
        """
        if cmd is None:
            # "None" command should only be sent by stop
            return False
        try:
            self.cmd_queue.put_nowait(cmd)
            return True
        except queue.Full:
            return False

    def start(self) -> None:
        """
        Start command processing thread
        """
        self.cmd_worker.start()

    def stop(self) -> None:
        """
        Stop command processing thread
        """
        self.cmd_queue.put(None)
        self.cmd_worker.join()

    def run(self) -> None:
        """
        Command processing thread function
        """
        while True:
            cmd = self.cmd_queue.get()
            # "None" command is considered a stop signal
            if cmd is None:
                break
            with self.cmd_rate_limiter:
                cmd.execute()


__all__ = [
    "Command",
    "MessageCommand",
    "CommandRunner",
]
