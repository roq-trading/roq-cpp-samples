#!/usr/bin/env python

import asyncio

import websockets


async def hello():
    async with websockets.connect('ws://localhost:2345') as websocket:
        try:
            await websocket.send('{"hello":"roq!"}')
            msg = await websocket.recv()
            print(msg)
        except websockets.ConnectionClosedOK:
            print("closed ok")
        except websockets.ConnectionClosedError:
            print("closed error")
        except websockets.ConnectionClosed:
            print("closed")
        print("done")


if __name__ == "__main__":
  asyncio.run(hello())
