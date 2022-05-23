"""
PyZMQ REQ socket client example module.

This is the client script for the zmq_server_rep_pthread program (@ examples/c/zmq_demo).
"""

import time
import zmq
import struct


def main():
    port = 5555;
    context = zmq.Context()
    print("Connecting to server...")
    client = context.socket(zmq.REQ)
    with client.connect(f"tcp://localhost:{port}"):
        for i in range(10):
            # Send request
            # Assuming little-endian in C side
            req_type = 2
            req_val = 42 + i
            req = struct.pack('<BI', req_type, req_val)
            client.send(req)

            # Receive response
            rep = client.recv()
            rep_val_a, rep_val_b = struct.unpack('<QB', rep)
            print(f"Received response [val_a: {rep_val_a}, val_b: {rep_val_b}]")


if __name__ == "__main__":
    main()
