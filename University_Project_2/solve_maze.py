'''
Author       : Ho Suk Lee (Max)
Email        : maxisbac@gmail.com
Last Modified: 10/MAY/15
'''

from header import DIRECTIONS, UP, RIGHT, DOWN, LEFT
from header import get_entry_point, get_exit_point


def solve_maze(maze):
    """
    solve_maze attempts to find the path through the maze in terms of list of
    2-tuples where each tuple represents grid coordiantes (y, x). Tuples are
    arranged in sequece of moves, starting from entry until the end.
    If no path is found, function returns None.
    Path never contains cycles.

    Assumptions: maze is a valid maze.

    Further details regarding the operation are commented within.
    """

    # maze will be solved by deploying 'robots' and recording their movements.
    # If a 'robot' meets a multi-pathing grid, further 'robots' are deployed.
    # Robot will stop moving until it is impossible for it to move,
    # ie. surrounded by walls or its path.
    # or when it reaches the exit.

    griddict = {UP: (-1, 0), DOWN: (1, 0), RIGHT: (0, 1), LEFT: (0, -1)}
    # For condition calculations

    MAZE_WID = len(maze)
    MAZE_LEN = len(maze[0])
    START = get_entry_point(maze)
    END = get_exit_point(maze)

    robots = [[START]]
    # Each list in robots represent a 'robot'.
    # Each 'robot' is a list of tuples, representing its pathway.

    finished_robots = []  # Robots unable to move further are moved here.
    rtnlst = []  # Final list after filtering

    def valid_grid(y, x):
        """
        valid_grid check if given grid cooridinates are within the maze.
        Returns True if valid, False otherwise.

        Assumptions: y and x are integers.
        """
        return y >= 0 and y < MAZE_WID and x >= 0 and x < MAZE_LEN

    while robots:  # find every possible way therefore loop all robots
        path = robots[0]
        (y, x) = path[-1]
        didnt_move = True  # condition to check if robot is trapped

        for dirc in DIRECTIONS:
            # Calculate the new grid details
            new_y, new_x = y + griddict[dirc][0], x+griddict[dirc][1]
            newgrid = (new_y, new_x)

            valid = valid_grid(new_y, new_x)

            if valid and maze[y][x][dirc] and newgrid not in path:
                # Check valid first for efficiency
                didnt_move = False
                if newgrid == END:
                    finished_robots.append(path+[newgrid])
                else:
                    robots.append(path+[newgrid])
                    # Deploy robot to grid, with history of its path

        # Reach the end or trapped, move to finished_robots.
        if didnt_move or END in path:
            finished_robots.append(path)

        robots = robots[1:]  # 'destroy' current robot.

    for robo in sorted(finished_robots):  # sorted in case of multiple paths
        if END == robo[-1]:
            rtnlst.append(robo)
            # Filter out only the robot that reached the end

    if rtnlst == []:
        return None
    return rtnlst[0]
    # In case of multiple pathways, return shortest.

