'''
Author       : Ho Suk Lee(Max)
Email        : maxisbac@gmail.com
Last Modified: 15/APR/15
'''


def pinary(n):
    """
    pinary is a function that returns value pi,
    estimated using the following formula by mathematician Leibniz :
    pi=4*[Sum[(−1)**k/(2k+1)]]=4(1 − 1/3 + 1/5 − 1/7 + 1/9 − ...)
    The parameter n determines when the series stops.
    (n+1 is the number of elements in the bracket)

    Assumptions: the argument is non-negative integer.
    
    Further details regarding the operation are commented within.
    """

    isum = 0  # This will be the series in the bracket (1-1/3+1/5..)
    for k in range(0, n+1):
        isum += (-1)**k / (2*k+1)  # Using the formula, add on p
    return 4*isum
