import threading
import queue
from abc import ABC, abstractmethod
from ratelimiter import RateLimiter  # type: ignore
from typing import Optional


class Command(ABC):
    @abstractmethod
    def execute(self) -> None:
        pass


class MessageCommand(Command):
    def __init__(self, msg: str) -> None:
        self.msg = msg

    def execute(self) -> None:
        print(f"MessageCommand: {self.msg}")


class CommandRunner:
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
        try:
            self.cmd_queue.put_nowait(cmd)
            return True
        except queue.Full:
            return False

    def start(self) -> None:
        self.cmd_worker.start()

    def stop(self) -> None:
        self.cmd_queue.put(None)
        self.cmd_worker.join()

    def run(self) -> None:
        while True:
            cmd = self.cmd_queue.get()
            if cmd is None:
                break
            with self.cmd_rate_limiter:
                cmd.execute()

__all__ = [
    "Command",
    "MessageCommand",
    "CommandRunner",
]
