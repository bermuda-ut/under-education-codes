'''
Author       : Ho Suk Lee(Max)
Email        : maxisbac@gmail.com
Last Modified: 15/APR/15
'''


def is_square_sum(num):
    """
    is_square_sum is a function that returns True
    if sum of squre of decomposition of the numbers
    equate to the squre of the number.
    Otherwise, it returns False.

    Assumptions: the argument is a single, non-neagtive integer.
    
    Further details regarding the operation are commented within.
    """

    LGTH = len(str(num))
    DENOM = [x for x in range(1, LGTH+1) if LGTH % x == 0]
    # DENOM is the list of possible numbers to decompose num without leftovers.
    # ex. 8833 can be decomposed by 1,2,4. denom = [1,2,4]
    #     3 is not in denom as decomposition becomes 883,3 and 3 is a leftover.

    for div in DENOM:
        numstr = str(num)
        cutlist = []  # List of decomposed elements
        isum = 0  # Sum of the square of decomposed elements

        while numstr:  # destructively go through numstr
            cutlist.append(int(numstr[:div]))
            numstr = numstr[div:]

        for element in cutlist:  # Sum the squares
            isum += element**2
        if isum == num:
            return True

    return False
