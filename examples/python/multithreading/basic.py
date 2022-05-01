import time
import threading


def thread_function(index: int, n_secs: int):
    """
    Example thread function that prints start and finish

    :param int index: Identifier for the thread.
    :param int n_secs: Number of seconds to sleep.
    """
    print(f"Thread {index}: starting")
    time.sleep(n_secs)
    print(f"Thread {index}: finishing")


def main():
    threads = list()
    n_threads = 3
    n_sec = 5

    for index in range(n_threads):
        print("Main: create and start thread", index)
        t = threading.Thread(target=thread_function, args=(index,n_sec))
        threads.append(t)
        t.start()

    for index, thread in enumerate(threads):
        print(f"Main: before joining thread {index}")
        thread.join()
        print(f"Main: thread {index} done")


if __name__ == "__main__":
    main()
