#!/usr/bin/env python

import asyncio

import websockets


async def hello():
    async with websockets.connect('ws://localhost:2345') as websocket:
        try:
            await websocket.send('{"action":"subscribe","symbol":"BTC-PERPETUAL"}')
            msg = await websocket.recv()
            print(msg)
            await websocket.send('{"action":"subscribe","symbol":"ETH-PERPETUAL"}')
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
