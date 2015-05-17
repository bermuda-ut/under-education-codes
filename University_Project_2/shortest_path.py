'''
Author       : Ho Suk Lee (Max)
Email        : maxisbac@gmail.com
Last Modified: 10/MAY/15
'''

from header import DIRECTIONS, UP, RIGHT, DOWN, LEFT
from header import get_entry_point, get_exit_point
from header import path_find, only_mins


def shortest_paths(maze, point):
    """
    shortest_paths returns list of sequence of moves to solve the maze
    by going through the point.
    The path never contains cycles.

    Assumptions: argument maze is a valid maze.

    Further details regarding the operation are commented within.
    """

    ENT = get_entry_point(maze)
    EXT = get_exit_point(maze)
    path1 = path_find(maze, ENT, point)
    # list of possible paths from entrance to the point.
    path2 = []
    # list of possible paths from point to exit.
    # This is calculated relative to path1, variations are in list.
    # Example : path2[0] = list of all possible paths from point
    #           to exit for path1[0]
    rtnlst = []  # final list to return

    for y in range(0, len(path1)):
        path2.append(path_find(maze, point, EXT, path1[y]))
        # for every path in path1, find possiblity and append to
        # list within path2.

    # Merge path1 and path2 if path2 exsists.
    for x in range(0, len(path1)):
        for a in range(0, len(path2[x])):
            if path2[x][a] != []:
                rtnlst.append(path1[x]+path2[x][a][1:])
                # [1:] to remove duplicate of point.

    if rtnlst == []:
        return None

    if len(rtnlst) > 1:
        rtnlst = only_mins(rtnlst)
        # filter out longer paths.

    return sorted(rtnlst)

