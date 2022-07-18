#!/usr/bin/env python

import asyncio

import websockets


async def hello():
    async with websockets.connect('ws://localhost:2345') as websocket:
        await websocket.send('{"hello":"roq!"}')
        msg = await websocket.recv()
        print(msg)


if __name__ == "__main__":
  asyncio.run(hello())
