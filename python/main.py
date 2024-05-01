import argparse
import logging
import os
import sys
import time

import numpy as np
import pandas

from BTinterface import BTInterface
from maze_2 import Action, Maze
from score import ScoreboardServer, ScoreboardFake

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)

log = logging.getLogger(__name__)

# TODO : Fill in the following information
TEAM_NAME = "第三組"
SERVER_URL = "http://140.112.175.18:5000/"
MAZE_FILE = "data/big_maze_112.csv"
BT_PORT = "COM5"

# python main.py --maze-file="data/small_maze.csv" --bt-port="21"`` --team-name="HELLO" --server-url="http://140.112.175.18:5000/" 1
# python main.py 1

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("mode", help="0: treasure-hunting, 1: self-testing", type=str)
    parser.add_argument("--maze-file", default=MAZE_FILE, help="Maze file", type=str)
    parser.add_argument("--bt-port", default=BT_PORT, help="Bluetooth port", type=str)
    parser.add_argument(
        "--team-name", default=TEAM_NAME, help="Your team name", type=str
    )
    parser.add_argument("--server-url", default=SERVER_URL, help="Server URL", type=str)

    return parser.parse_args()


def main(mode: int, bt_port: str, team_name: str, server_url: str, maze_file: str):
    #comment!!!
    maze = Maze(maze_file)
    interface = BTInterface(port=bt_port)
    point = ScoreboardServer(team_name,server_url)
    #point = ScoreboardFake("your team name", "data/fakeUID.csv") # for local testing
    # TODO : Initialize necessary variables

    if mode == "0":
        log.info("Mode 0: For treasure-hunting")
        # TODO : for treasure-hunting, which encourages you to hunt as many scores as possible

    elif mode == "1":
        log.info("Mode 1: Self-testing mode.")
        # TODO: You can write your code to test specific function.
        start = maze.get_node_dict()[6]
        goal = maze.BFS(start)
        path = maze.BFS_2(start, goal)
        cmd = maze.actions_to_str(maze.getActions(path))
        cmd = "f" + cmd + "?"
        time.sleep(5)
        interface.send_action(cmd[0:3])
        counter = 3
        while True:
            s = interface.receive_message()
            print(s)
            if s[0:3] == "get":
                print("I get!")
                if counter >= len(cmd):
                    print("I break!")
                    break
                else: 
                    interface.send_action(cmd[counter])
                    counter = counter + 1
                
                
        """for i in range(0, len(cmd)):
            interface.send_action(cmd[i])"""
        while True:
            s = interface.receive_message()
            if len(s) >= 8:
                print(s[0:8])
                point.add_UID(s[0:8])
                point.get_current_score()
                start = goal
                goal = maze.BFS(start)
                if start != goal:
                    path = maze.BFS_2(start, goal)
                    cmd = maze.actions_to_str(maze.getActions(path))
                    cmd = cmd + "?"
                    interface.send_action("b" + cmd[1:3])
                    counter = 3
                    while True:
                        s = interface.receive_message()
                        if s[0:3] == "get":
                            if counter >= len(cmd):
                                break
                            else: 
                                interface.send_action(cmd[counter])
                                counter = counter + 1
                    if not point.socket.connected:
                        interface.send_action("g")
                        break
                else: 
                    point.socket.disconnect()
                    interface.send_action("g")
                    break
        
        """
        for i in range(1, 12):
            start = goal
            goal = maze.BFS(start)
            path = maze.BFS_2(start, goal)
            cmd = maze.actions_to_str(maze.getActions(path))
        """
        

    else:
        log.error("Invalid mode")
        sys.exit(1)


if __name__ == "__main__":
    args = parse_args()
    main(**vars(args))