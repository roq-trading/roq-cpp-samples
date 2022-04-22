#!/usr/bin/env python

import zmq

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://localhost:1234")

socket.setsockopt_string(zmq.SUBSCRIBE, "[")

while True:
    msg = socket.recv()
    print(msg)
