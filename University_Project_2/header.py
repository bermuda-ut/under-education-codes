'''
Author       : Ho Suk Lee (Max)
Email        : maxisbac@gmail.com
Last Modified: 10/MAY/15
'''

DIRECTIONS = (UP, RIGHT, DOWN, LEFT) = (0, 1, 2, 3)


def get_exit_point(maze):
    """
    get_exit_point takes one argument maze, returns 2-tuple of integers
    identifying the maze exit point.

    Assumptions: maze is a valid maze.
    """

    MAZE_LEN = len(maze[0])
    MAZE_WID = len(maze)

    for x in range(0, MAZE_LEN):
        if maze[-1][x][DOWN]:
            return (MAZE_WID-1, x)


def get_entry_point(maze):
    """
    get_entry_point takes one argument maze, returns 2-tuple of integers
    identifying the maze entry point.

    Assumptions: maze is a valid maze.
    """

    MAZE_LEN = len(maze[0])

    for x in range(0, MAZE_LEN):
        if maze[0][x][UP]:
            return (0, x)


def path_find(maze, start, end, visited=[]):
    """
    path_find returns sorted list of possible paths from the starting to the
    ending point in the maze, both set by two arguments start and end.
    visited, default empty list, is a list of grids that can be assgined to
    prevent paths going through the grids.

    Assumptions: maze is a valid maze.

    Further details regarding the operation are commented within.
    """

    # path will be solved by deploying 'robots' and recording their movements.
    # If a 'robot' meets a multi-pathing grid, further 'robots' are deployed.
    # Robot will stop moving until it is impossible for it to move,
    # ie. surrounded by walls or its path.
    # or when it reachs the end point.

    griddict = {UP: (-1, 0), DOWN: (1, 0), RIGHT: (0, 1), LEFT: (0, -1)}
    # For new grid condition calculations

    MAZE_WID = len(maze)
    MAZE_LEN = len(maze[0])

    robots = [[start]]
    # Each list in robots represent a robot.
    # Each robot is a list of tuples, representing its pathway.

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
            new_y, new_x = y+griddict[dirc][0], x+griddict[dirc][1]
            newgrid = (new_y, new_x)

            cantgo = path + visited
            valid = valid_grid(new_y, new_x)

            if valid and maze[y][x][dirc] == True and newgrid not in cantgo:
                # Check valid first for efficiency
                didnt_move = False
                if newgrid == end:
                    finished_robots.append(path+[newgrid])
                else:
                    robots.append(path+[newgrid])
                    # Deploy robot to grid, with history of its path

        if didnt_move or end in path:
            finished_robots.append(path)
            # Reach the end or trapped, move to finished_robots.

        robots = robots[1:]  # 'destroy' current robot.

    # Filter out only the robot that reached the end
    for item in finished_robots:
        if end == item[-1]:
            rtnlst.append(item)

    return sorted(rtnlst)


def only_mins(lst):
    """
    only_mins returns lists in lst with minium length.

    Assumptions: lst is not empty and contains lists.
    """

    minlst = [lst[0]]

    for item in lst[1:]:
        if len(item) < len(minlst[0]):
            minlst = [item]
        elif len(item) == len(minlst[0]):
            minlst.append(item)
            # Add to list if equal length

    return minlst

