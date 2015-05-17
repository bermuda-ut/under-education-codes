'''
Author       : Ho Suk Lee(Max)
Email        : maxisbac@gmail.com
Last Modified: 15/APR/15
'''


def nearagram(word1, word2):
    """
    nearagram returns number n for n-neargram between two words.
    ex. apple and papal. apple has extra e and missing a therefore
        apple is 2-neargram of papal. nearagram returns 2.
    This will be achieved by using the concept of taking both words to a
    equal point(word).

    Assumptions: the arguments are lower-case, non-empty string.
    
    Further details regarding the operation are commented within.
    """

    if len(word1) < len(word2):  # Ensures word1 is always longer.
        word1, word2 = word2, word1

    extList = list(word1)
    for letter in word2:
        if letter in extList:
            extList.remove(letter)
    # extList is list of extra letters in word1.
    # Removal rather than appending used to take account of multiple letters.

    extra2 = len(extList)+len(word2)-len(word1)

    # (word1) - (extra for word1) = (word2) - (extra for word2)
    # ex. apple - pple = aaa - aa. Both reach equal word of 'a'
    # By this, we know that:
    # len(aa) = len(aaa) - len(apple) + len(pple)
    #
    # Therefore by letting extra2 = len(extra for word2), the above
    # extra2 equation is achieved.

    return len(extList) + extra2
    # Two words reached equal point therefore must add them together.
