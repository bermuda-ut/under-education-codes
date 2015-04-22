'''
Author       : Ho Suk Lee(Max)
Email        : maxisbac@gmail.com
Last Modified: 15/APR/15
'''


def jp_morify(jp_word):
    """
    jp_morify decomposes and returns the word into Japanese morae.
    This is based on Kunrei-shiki system.
    If the arguemnt is not a valid morae, function returns None

    Assumptions: the argument is non-empty string.

    Further details regarding the operation are commented within.
    """

    VOWEL = [x for x in 'aeiou']
    MORA = VOWEL + ['N']
    SCONST = [y for y in 'kgsztdnhbpmyw']  # Single Consonants
    DCONST = [z+'y' for z in 'kgsztdnhbpm']  # Double Consonants
    NOTVAL = ['ye', 'yi', 'we', 'wi', 'wu']  # Impossible combinations.

    rtnlist = []  # List that will contain decomposed words

    if any(ltr in jp_word for ltr in NOTVAL) or jp_word[0] == 'N':
        return None
    # Check if impossible combination in jp_word or starts with N
    # Essential since following code does not detect this.

    while jp_word:  # Go through the word destructively
        if jp_word[:2] in DCONST and jp_word[2] in VOWEL:
            rtnlist.append(jp_word[:3])
            jp_word = jp_word[3:]
            # First check if double consonant + vowel.

        elif jp_word[0] in SCONST and jp_word[1] in VOWEL:
            rtnlist.append(jp_word[:2])
            jp_word = jp_word[2:]
            # Check if single consonant + vowel.
            # This must come after checking double constant+vowel since
            # double constants' first letter are single constants.

        elif any(l in MORA for l in jp_word[0]):
            rtnlist.append(jp_word[0])
            jp_word = jp_word[1:]
            # Finally check if mora.

        else:
            return None
            # If none of above, not a valid Japanese word.

    return rtnlist
