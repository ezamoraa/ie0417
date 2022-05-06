import time
import queue
from threading import Thread, Event
from typing import Optional

class Message:
    def __init__(self, txt: str):
        self.evt = Event()
        self.txt = txt

producer_txts = [
    "Programmer: A machine that turns coffee into code",
    "Computers are fast; programmers keep it slow",
    "When I wrote this code, only God and I understood what I did. Now only God knows",
    "It’s not a bug — it’s an undocumented feature",
    "It works on my machine",
    "It compiles; ship it",
    "When all else fails … reboot"
]

def msg_producer(q):
    print("Producer thread starting")
    time.sleep(0.5)

    def send_msg_wait(txt: str, timeout=None):
        msg = Message(txt)
        print("Sending message to thread")
        begin = time.time()
        q.put(msg)
        wait_ok = msg.evt.wait(timeout)
        elapsed = time.time()-begin
        if not wait_ok:
            print("Timed out waiting for event")
        print(f"Elapsed time (s): {elapsed}\n")

    for txt in producer_txts:
        send_msg_wait(txt)
    # Stop consumer from reading messages
    q.put(None)

    print("Producer thread finishing")


def msg_consumer(q):
    print("Consumer thread starting")

    while True:
        msg = q.get()
        # "None" is considered a stop signal
        if msg is None:
            break
        print(f"Processing message: \"{msg.txt}\"")
        time.sleep(0.5)
        msg.evt.set()

    print("Consumer thread finishing")


def main():
    q = queue.Queue()

    t_p = Thread(target=msg_producer, args=(q,))
    t_c = Thread(target=msg_consumer, args=(q,))
    t_p.start()
    t_c.start()
    t_p.join()
    t_c.join()


if __name__ == "__main__":
    main()
