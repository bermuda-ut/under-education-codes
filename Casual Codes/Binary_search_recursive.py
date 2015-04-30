'''
Author       : Ho Suk Lee (Max)
Email        : maxisbac@gmail.com
Last Modified: 01/MAY/15
'''

def bsearch(lst, key, idx=0):
    if lst:
        mid = len(lst)//2

        if lst[mid] == key:
            return (mid+idx)
        if lst[mid] < key:
            return bsearch(lst[mid+1:], key, mid+idx+1)
            # getting rid of mid here saves the trouble of creating condition for lst[0] != key...
        return bsearch(lst[:mid], key, idx)

    return False