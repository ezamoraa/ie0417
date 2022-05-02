import time
import threading

from pprint import pprint
from typing import Callable

g_counter: int = 0
lock = threading.Lock()


def incr_unlocked(n: int):
    """
    Increment global counter without locking.

    :param int n: Number of increments.
    """
    global g_counter
    for i in range(n):
        g_counter += 1


def incr_locked(n: int):
    """
    Increment global counter with locking.

    :param int n: Number of increments.
    """
    global g_counter
    for i in range(n):
        lock.acquire()
        g_counter += 1
        lock.release()


def increment_mt_experiment(
        incr_func: Callable[[int], None],
        n_threads: int,
        n_incr: int):
    """
    Multithreading experiment that increments a global counter
    in multiple threads

    :param func incr_func: The function that increments the counter.
    :param int n_threads: The number of threads to spawn.
    :param int n_incr: The number of increments to perform.
    """
    global g_counter
    g_counter = 0
    threads = [
        threading.Thread(target=incr_func, args=(n_incr,))
        for _ in range(n_threads)
    ]

    print(f"Starting increment MT experiment in {n_threads} threads."
          f" func={incr_func.__name__}")

    begin = time.time()
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()

    elapsed = time.time()-begin
    expected = n_incr * n_threads
    diff = expected - g_counter
    diff_p = 100 * diff/expected

    results = {
        "counter": g_counter,
        "difference (abs)": diff,
        "difference (%)": diff_p,
        "elapsed (s)": elapsed,
        "expected": expected,
    }
    pprint(results)
    print()


def main():
    num_threads: int = 10
    num_increments: int = 100000
    increment_mt_experiment(incr_unlocked, num_threads, num_increments)
    increment_mt_experiment(incr_locked, num_threads, num_increments)


if __name__ == "__main__":
    main()
