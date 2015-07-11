'''
Author      : Max Lee
Email       : hoso1312@gmail.com
Last Edited : 11/JUL/15

Hearts Cardgame AI information
AI Name     : Lawliet
Gender      : null
Play Style  : Standard n00b - Max
'''

splashtext = """
Whistful Hearts AI : Lawliet : by Ho Suk Lee

     .------..------..------.
     |G.--. ||E.--. ||T.--. |
     | :/\: || (\/) || :/\: |
     | :\/: || :\/: || (__) |
     | '--'G|| '--'E|| '--'T|
     `------'`------'`------'
   __                _ _      _
  / /  __ ___      _| (_) ___| |_
 / /  / _` \ \ /\ / / | |/ _ \ __|
/ /__| (_| |\ V  V /| | |  __/ |_
\____/\__,_| \_/\_/ |_|_|\___|\__|

 .------..------..------..------.
 |R.--. ||E.--. ||K.--. ||T.--. |
 | :(): || (\/) || :/\: || :/\: |
 | ()() || :\/: || :\/: || (__) |
 | '--'R|| '--'E|| '--'K|| '--'T|
 `------'`------'`------'`------'

AI NAME  : Lawliet
FATHER   : Ho Suk Lee
COMMENTS : Tim's attempt to get me to evangelise wherever I go.
           He clearly failed, as reflected in the comments and splash text.
"""

print(splashtext)

# Basic cards data shared across functions
SUIT = ['C', 'H', 'D', 'S']
CNUM = [str(x) for x in range(2, 10)] + ['0', 'J', 'Q', 'K', 'A']
CARDS = []

# creation of deck
for st in SUIT:
    for num in CNUM:
        CARDS.append(num+st)


def is_broken_hearts(prev_tricks, curr_trick=()):
    '''
    Returns True if heart has been 'broken' up to the point.
    Returns False otherwise.

    Accepts following two arguments:
    prev_tricks - list of 4 tuples of cards, each form of string,
                  in completed tricks in order of play.
    curr_trick - tuple of cards that have been played in current in
                 complete trick. It has default value of () - empty tuple.
    '''

    if len(prev_tricks) > 3:  # not preliminery stage
        for trick in prev_tricks[3:]:
            for card in trick:
                if 'H' in card:
                    return True

        if curr_trick:
            for card in curr_trick:
                if 'H' in card:
                    return True
    return False


def is_valid_play(play, curr_trick, hand, prev_tricks,\
                  broken=is_broken_hearts):
    '''
    Returns True if the play is valid, returns False otherwise.

    Accepts following arguments:
    play - single card representing potential play.
    curr_trick - tuple of cards that have been played in the current round.
    hand - list of cards the player currently holds.
    prev_tricks - list of completed tricks for the current game.
    broken has default value of the function is_broken_hearts. This is for
    testing and ensuring no errors pass over when marking.

    Assumption : All arguments are individually valid, size of hand is
    consistent with size of prev_tricks
    '''

    if play not in hand:
        return False

    if curr_trick and curr_trick[0][1] != play[1]:
        # Not following lead suit
        # Check if the player ran out of the suit
        for card in hand:
            if curr_trick[0][1] == card[1]:
                return False
    elif len(prev_tricks) > 2 and play[1] == 'H' and \
       not broken(prev_tricks,curr_trick):
        for card in hand:
            if card[1] != 'H':
                return False

    return True


def greatest(trick):
    '''
    Returns a single card in string that has the greatest number.
    Does not take account of suits.

    Accepts single argument trick.
    '''
    winner = trick[0]
    if len(trick) > 1:
        for card in trick[1:]:
            if cardnum(card) > cardnum(winner):
                winner = card
    return winner


def cardnum(card):
    '''
    Returns digit value of the card number in integer.
    Greater the card number, greater the return value.
    ie. 2 returns smallest, A returns greatest.
    '''
    return CNUM.index(card[0])


def trick_winner(trick, playorder, trump=''):
    '''
    Returns a tuple that has (trick winner, new play order)
    Trick winner is self explanatory.
    New play order is a list of players in their play order for the next
    game.

    Accepts following arguments:
    tricks - list of 4-tuples, each representing a single trick, in order
             of play, starting with the lead card.
             Tricks are from 0 to 13 inclusive
    playorder - list of integers representing each players. This is ordered
                in the current play order.
    trump - single string representing the trump of the trick. This argument
            is used for preliminery stage.
            It has default value of null.
    '''

    if trump == '':
        # Not preliminery stage, trump is the first card in the trick.
        trump = trick[0]

    total = ''  # String that will contain all the suits used
    for card in trick:
        total += card[1]

    if trump[1] in total:
        # If anyone played the trump suit.
        # All hearts-round games will end up here.
        filtered = []  # list containing cards with lead suit.
        for card in trick:
            if trump[1] in card:
                filtered.append(card)

        winner = greatest(filtered)
        windex = trick.index(winner)
        playorder = playorder[windex:]+playorder[:windex]
        return (winner, playorder)
    else:
        # If nobody has played the trump - possible in preliminery stage
        return trick_winner(trick, playorder)
        # Try again without trump, as stated in the rules.


def score_game(tricks, deck_top):
    '''
    Returns 4-tuple of integers represeiting score for each four players.
    The order of players in the tuple is in order of first trick played.
    All subsequent tricks follow same ordring but start with winner of
    preceding trick.
    The function is callable at any stage during the game.

    Accepts following two arguments:
    tricks - list of 4-tuples, each representing a single trick, in order
             of play, starting with the lead card.
             Tricks are from 0 to 13 inclusive
    deck_top - list of cards that were turned up on top of the deck for the
               tricks played to date (including current trick).
               deck_top from 1 to 3 inclusive

    Assumption : Length of deck_top is appropriate for size of tricks
    '''

    playorder = [0, 1, 2, 3]  # Starting play order of players.
    wincards = [[], [], [], []]  # Their cards won
    rtnscr = [0, 0, 0, 0]  # Final scores to return. Ordered in playorder above
    minusHeart = False  # Whether hearts should be considered as minus points
    heartcheck = rtnscr + []
    # list of integers that represents the presence of hearts won for players.
    # If the player has won hearts, the integer value for their index becomes 1

    if len(tricks) < 3:
        return tuple(rtnscr)  # no score possible, preliminery stage.

    for tn in range(len(tricks)):
        if tn < 3:
            playorder = trick_winner(tricks[tn], playorder, deck_top[0])[1]
            # Update play order according to preliminery stage.
        else:
            playorder = trick_winner(tricks[tn], playorder)[1]
            wincards[playorder[0]] += list(tricks[tn])
            # Update play order and card won for each players.

    # Update heartcheck
    for x in range(4):
        for card in wincards[x]:
            if 'H' in card:
                heartcheck[x] = 1
                break

    # Check the number of people with hearts
    total = 0
    for items in heartcheck:
        if items == 1:
            total += 1

    # If only one, proves that person has won all the cards
    # therefore hearts should be minus points.
    if total == 1:
        minusHeart = True

    # Points calculation for each players.
    for x in range(4):
        for card in wincards[x]:
            if 'H' in card:
                if minusHeart:
                    rtnscr[x] -= 1
                else:
                    rtnscr[x] += 1
            if card == 'QS':
                rtnscr[x] += 13

    # Return in tuple as required.
    return tuple(rtnscr)


def play(curr_trick, hand, prev_tricks, deck_top, is_valid=is_valid_play,\
         score=score_game, player_data=None, suppress_player_data=False):
    '''
    Determines most suitable card to play from hand in current situation and
    returns the card(string).
    If bonous round (ie. player_data != None), 2-tuple is returned,
    (score, player_data) where player_data is the predicted score.

    Current situation is analyzed by extracting dta from given arguements.
    Further details are commented within.

    In basic, the function flows as follows:
    Data extraction > specific strategy for current turn > general strategy

    Accepts following arguements :
    curr_trick - tuple containing cards playd in the current incomplete trick
    hand - list of cards that you curretly hold
    prev_tricks - list of 4 tuples of cards, each form of string,
                  in completed tricks in order of play.
    deck_top - list of strings representing the cards that were turned up
               for the tricks plated to date including current
    is _valid has default value of the function is_valid_play. This is for
    testing and ensuring no errors pass over when marking.
    player_data - store any data used in formulating my stategy. Only used
                  in bonus round. This defaults to the optionally-returned
                  player_data from the immediately preciding call to play,
                  which is the predicted score in first call of play.
    suppress_player_data - supresss generation of player_data - for testing

    Assumptions : Cards are in string format, every arguments are valid.
    '''

    import copy  # Need to copy a list for possibility calculations

    #
    # Functions
    #
    def arrange_big(hand):
        """
        Quicksort hand so that big card has big index number.
        Better to understand this way rather than ascending order for
        overall readability and understanding the code.
        """
        left = []  # less than pivot
        centre = []  # pivots
        right = []  # more than pivot
        if len(hand) > 1:
            pivot = cardnum(hand[0])
            for card in hand:
                cval = cardnum(card)  # convert to integer value

                if cval < pivot:
                    left.append(card)
                elif cval > pivot:
                    right.append(card)
                elif cval == pivot:
                    centre.append(card)

            return arrange_big(left) + centre + arrange_big(right)
        else:
            return hand  # basecase return

    def canwin(curr_list, hand):
        """
        Returns Flase if unable to win the trick
        if able to win the trick, return smallest card with win condition

        Accepts following two arguments:
        curr_list - tuple of current trick, with string representing cards.
        hand - player's hand, list of string representing cards.
        """
        if curr_list:
            lead = curr_list[0]

            following = False
            for card in hand:
                if card[1] == lead[1]:
                    following = True
                    break
            if not following:
                return False

            for card in arrange_big(hand):
                if card[1] == lead[1] and \
                   cardnum(card) > cardnum(greatest(curr_list)):
                    return card
        return False

    def canlose(curr_list, hand):
        """
        Return False if unable to lose the trick
        if able to win the trick, check if following the suit, if follow,
        return any greatest card. else, return losable card.

        only takes account of curr_list, not possibilities.

        Accepts following two arguments:
        curr_list - tuple of current trick, with string representing cards.
        hand - player's hand, list of string representing cards.
        """
        if curr_list:
            leadsuit = curr_list[0][1]

            following = False  # Following means following the lead suit.
            for card in hand:
                if card[1] == leadsuit:
                    following = True
                    break

            if following:
                curr_list = [x for x in curr_list if x[1]==leadsuit]
                for card in arrange_big(hand)[::-1]:
                    if card[1] == leadsuit and\
                       cardnum(card) < cardnum(greatest(curr_list)):
                        return card
            else:
                for card in arrange_big(hand)[::-1]:
                    if card[1] != leadsuit:
                        # return greatest that is not leadsuit
                        # since not following
                        return card
        return False

    def guranteed_win(suit, valids):
        """
        Function relates valids and possibles(what others have) and
        calculates whether player is guranteed to win the trick.

        Accepts following two arguments:
        curr_list - tuple of current trick, with string representing cards.
        hand - player's hand, list of string representing cards.
        """
        others = arrange_big(possidict[suit])[::-1]  # descending order
        mincard = arrange_big(valids)[0]  # smallest card
        greaters = 0  # number of card that is greater value than minimum

        for card in others:
            if cardnum(card) > cardnum(mincard):
                greaters += 1
            else:
                break
                # since ordered dscending, guranteeed following values are
                # smaller or equal. Therefore can break.
        if greaters / len(others) >= LOSSCARDRATIO:
            # Must be above certain ratio to consider able to lose.
            return False
        return True

    def attempt_lose():
        """
        Function attempts to lose the trick.
        If succees, a suitable card is returned.
        Otherwise, False is returned.
        """
        if req_check:
            # checks if new sets of valids must be considered.
            new_can_lose = canlose(curr_trick, valids)
            if new_can_lose:
                return final(new_can_lose)
        elif can_lose:
            return final(can_lose)
        return False

    def attempt_win():
        """
        Function attempts to win the trick.
        If succees, a suitable card is returned.
        Otherwise, False is returned.
        """
        if req_check:
            # checks if new sets of valids must be considered.
            new_can_win = canwin(curr_trick, valids)
            if new_can_win:
                return final(new_can_win)
        elif can_win:
            return final(can_win)
        return False

    def cards_gained(tricks, deck_top):
        """
        Returns list of cards that each player has won
        The order is in 0,1,2,3.

        The two arguments, tricks and deck_top, have same conditions as the
        play function.
        """
        playorder = [0, 1, 2, 3]
        wincards = [[], [], [], []]

        for tn in range(0, len(tricks)):
            if tn < 3:
                playorder = trick_winner(tricks[tn], playorder, deck_top[0])[1]
            else:
                playorder = trick_winner(tricks[tn], playorder)[1]
                wincards[playorder[0]] += list(tricks[tn])

        return wincards

    def curr_playorder(tricks, deck_top):
        """
        Reurns list of player ids in their player order up to the trick.

        The two arguments, tricks and deck_top, have same conditions as the
        play function.
        """
        playorder = [0, 1, 2, 3]

        for tn in range(0, len(tricks)):
            if tn < 3:
                playorder = trick_winner(tricks[tn], playorder, deck_top[0])[1]
            else:
                playorder = trick_winner(tricks[tn], playorder)[1]
        return playorder

    def final(card):
        """
        As reflected in the function name, this function determines final
        return value, according to whether current game is bonous or not.

        Accepts single arugment card in string.
        """
        if suppress_player_data:
            return card
        return (card, player_data)

    broken = is_broken_hearts  # link to function

    #
    # Constants
    # Pretty much determines how the AI performs.
    #
    # Getting 'Magic' numbers for the following constants will require
    # extremely experienced and seasoned player, ie. not me.
    LOSSCARDRATIO = 0.55  # Ratio between cards with greater / total.
    # This is used to decide guranteed lost scenarios.
    HRATIO = 0.01 # Ratio between otherplayer hearts/my hearts. 
    # This is to determine ideal time to break heart
    BORDERRATIO = 0.4  # Ratio between small/total for a suit in hand.
    # below certain point, dangerous and need to discard with priority.
    VOIDTHRESH = 4  # number of cards below to consider able to void.

    #
    # Possible Cards Data
    #
    possibles = copy.copy(CARDS)  # Unique copy
    for trick in prev_tricks:  # cards not in prev_tricks
        for card in trick:
            possibles.remove(card)
    for card in curr_trick:  # not in current trick
        possibles.remove(card)
    for card in hand:  # not in player's hand
        possibles.remove(card)

    # Arrange by suits
    Hpossible = [x for x in possibles if x[1] == 'H']
    Spossible = [x for x in possibles if x[1] == 'S']
    Cpossible = [x for x in possibles if x[1] == 'C']
    Dpossible = [x for x in possibles if x[1] == 'D']
    # These are used in logic filters.

    possidict = {'H': Hpossible, 'S': Spossible,\
                 'C': Cpossible, 'D': Dpossible}
    # Used to calculate guranteed loss scnarios

    #
    # Player Data
    #
    player = len(curr_trick)  # my current player order
    allscore = score(prev_tricks, deck_top)
    # scores for each players, ordered in first trick order.
    myscore = allscore[player]
    allgain = cards_gained(prev_tricks, deck_top)  # cards each player gained
    mygain = allgain[player]

    enemygain = []  # compilation of total enemy gains
    for x in range(3):
        if x != player:
            enemygain += allgain[x]
    curr_order = curr_playorder(prev_tricks, deck_top)

    #
    # Hearts Data
    #
    myhearts = [x for x in hand if x[1] == 'H']

    outhearts = []  # Hearts that I don't have
    for card in possibles:
        if card[1] == 'H' and card not in myhearts:
            outhearts.append(card)

    denom = len(myhearts) + len(outhearts)
    if denom != 0:
        heartratio = len(myhearts) / denom  # ratio used for decision making

    else:
        heartratio = 0

    Hincurr = []  # List of Hearts in current trick
    if curr_trick:
        for card in curr_trick:
            if card[1] == 'H':
                Hincurr.append(card)

    #
    # Queen of Spades Data
    #
    QSactive = True  # QS is 'active' if someone is holding it.
    for pgain in allgain:  # if someone has gained it, QS is no longer active
        if 'QS' in pgain:
            QSactive = False
            break
    IhaveQS = 'QS' in hand  # Whether player is holding QS

    #
    # Player Valids Cards Data
    #
    valids = []
    for card in hand:
        if is_valid(card, curr_trick, hand, prev_tricks):
            valids.append(card)
    valids = arrange_big(valids)

    # Divide valids and hands according to suit
    # Mainly to make calculations shorter overall.
    Hvalids = [x for x in valids if x[1] == 'H']
    Svalids = [x for x in valids if x[1] == 'S']
    Cvalids = [x for x in valids if x[1] == 'C']
    Dvalids = [x for x in valids if x[1] == 'D']
    valbysuit = [Cvalids, Hvalids, Dvalids, Svalids]
    # order following SUITS

    Hhand = [x for x in hand if x[1] == 'H']
    Shand = [x for x in hand if x[1] == 'S']
    Chand = [x for x in hand if x[1] == 'C']
    Dhand = [x for x in hand if x[1] == 'D']
    handbysuit = [Chand, Hhand, Dhand, Shand]
    # order following SUITS

    smalls = [0, 0, 0, 0]  # number of 'small' cards.
    # order following SUITS
    for suitnum in range(4):
        count = 0
        for card in handbysuit[suitnum]:
            if cardnum(card) < 4:  # below number 5
                count += 1
        smalls[suitnum] = count

    handcount = {'H': len(Hhand), 'S': len(Shand),\
                 'C': len(Chand), 'D': len(Dhand)}
    valcount = {'H': len(Hvalids), 'S': len(Svalids),\
                'C': len(Cvalids), 'D': len(Dvalids)}
    # if suitnum index system is clunky, use dictionary.

    #
    # Variables
    #
    hand = arrange_big(hand)  # Re-order variables
    current = len(prev_tricks) + 1  # current play id (NOT player)
    can_lose = canlose(curr_trick, valids)  # shortcuts
    can_win = canwin(curr_trick, valids)
    req_check = False  # Original valids may alter thefore to keep track of it.

    # according to number of cards, arrange suit ascending order.
    suit_arr = []
    for (suit, count) in handcount.items():
        # 'linear' way to arrange.
        if suit_arr == []:
            suit_arr.append(suit)
            continue
        for num in range(len(suit_arr)):
            compare = suit_arr[num]
            if handcount[compare] >= handcount[suit]:
                if count > 0:
                    suit_arr.insert(num, suit)
                break

    # List of absent suits for each players
    all_absent = [[], [], [], []]
    str_absent = ""  # all_absent joined into string.
    tot_absent = []  # string of suits that is not present for all.

    for trick_id in range(3, len(prev_tricks)):
        playodr = curr_playorder(prev_tricks[:trick_id], deck_top)
        # specific trick's play order.
        # ex. Trick ID 4, slice prev tricks and exclude from ID 4,
        #     then pass on to curr_playorder, to obtain play order for ID 4.
        trick = prev_tricks[trick_id]  # Trick to consider
        lead = trick[0]

        for card_id in range(1, 4):
            card = trick[card_id]
            if card[1] != lead[1]:  # if card is not matching
                player_id = playodr[card_id]
                # match player id with currnt trick order.
                if not (lead[1] in all_absent[player_id]):
                    all_absent[player_id].append(lead[1])

    for suit in SUIT:
        counter = 0
        for tempplayer in all_absent:
            if suit in tempplayer:
                counter += 1
        if counter == 3:  # if all 3 other players do not have
            tot_absent.append(suit)

    for tempplayer in all_absent:
        str_absent += "".join(tempplayer)

    #
    # Action Priority Selection
    #
    priority = 0  # Default

    # If able to void a suit, make that suit a priority.
    for leastsuit in suit_arr:
        if leastsuit not in str_absent and valcount[leastsuit] > 1:
            # if nobody is void with that suit and suit present in valids
            if leastsuit == 'S':
                # for Spades, cannot void until zero.
                if 1 < count < VOIDTHRESH and Svalids and not IhaveQS:
                    priority = 4
                    break
            elif count < VOIDTHRESH:
                    priority = SUIT.index(leastsuit) + 1
                    break
                    # Refer to priority reference table below.

    if priority == 0:
        if Hhand and Hvalids:
            if cardnum(arrange_big(Hhand)[-1]) > 5:
                # if aim was to throw big, might aswell do Hearts.
                priority == 2

    # For each suits in hand, consider
    if priority == 0:
        for suitnum in range(4):
            if SUIT[suitnum] not in tot_absent:  # someone guranteed to follow
                total = len(handbysuit[suitnum])
                if total > 3 and valbysuit[suitnum] and\
                   smalls[suitnum]/total < BORDERRATIO:
                    # if there are a lot of big cards
                    priority = suitnum + 1
                    break

    '''
    Priority Reference -
    0  :  Default. Dump biggest(for early game)
    1  :  Dump C
    2  :  Dump H
    3  :  Dump D
    4  :  Dump S
    '''

    #
    # Decision making &
    # Logic Filters
    #

    # First, decide the stage of the game.
    # Divded into Preliminery, Early, Mid, Late game.
    # For Prelim and Early games, specifc customized strategy is used.
    # For Mid and Late games, share general stragey however go through
    # specific customized filter first.

    if len(prev_tricks) < 3:
        # prelim stage.
        # Actions taken here is universal for normal/bonus games

        if 'QS' in valids and smalls[3] < 4:
            return final('QS')

        if 'QS' in curr_trick:
            # Do not want to gain QS.
            if attempt_lose():
                return attempt_lose()

        if QSactive and deck_top[0][1] != 'S':
            for card in ['KS', 'AS']:
                if card in valids:
                    return final(card)
        # get rid of super dangerous safely.

        if priority == 0 or priority == 4:
            hand_dict = {'H': Hhand, 'S': Shand, 'C': Chand, 'D': Dhand}
            decktop_suit = deck_top[-1][1]
            decktop_value = cardnum(deck_top[-1])
            worth = False

            for card in hand_dict[decktop_suit]:
                if cardnum(card) > decktop_value:
                    worth = True
                    # if player has a card greater than topdeck, worth taking.

            if worth:
                if attempt_win():
                    return final(valids[-1])
                # Try to win the card since worthy

            if player > 1 and attempt_lose():
                return attempt_lose()
                # if plays revealed, worth trying to lose the trick
            return final(valids[-1])
            # else just return greatst.
        else:
            return final(arrange_big(valbysuit[priority-1])[-1])
            # return greatest for the priority value.
    # End of prelim strategy

    # check if player is following the leadsuit
    if curr_trick:
        leadsuit = curr_trick[0][1]
        valstring = "".join(valids)
        # Join valids to string to quickly figure out presnce of certain suit

        if leadsuit in valstring:
            following = True
        following = False
        # Following means following lead suit

    # Early game plays completely differently from Mid/Late game,
    # therefore has it's own complete unique logic filter.

    if len(prev_tricks) < 7 and not broken(prev_tricks, curr_trick):
        # Early game and Hearts has not been broken to be successful.
        # This stage, hearts not usualy broken and everyone is NOT void.
        # Statistically, during early stages of thegame, winning a trick
        # is mostly benificial
        # By leading the trick, you are able to choose which suit to void.
        if 'QS' in curr_trick:
            if attempt_lose():
                return attempt_lose()

        if QSactive and not IhaveQS:
            for danger in ['AS', 'KS']:
                if danger in valids and len(valids) > 1:
                    valids.remove(danger)
                    req_check = True
        # do not use KS AS for safety.

        if curr_trick:
            # if not lead
            if Hincurr:
                # Try to lose the trick if Heart is present
                if attempt_lose():
                    return attempt_lose()
                # if cannot lose
                # go through possibles. if guranteed win, discard greatest card
                if guranteed_win(leadsuit, valids):
                    return final(valids[-1])
                else:
                    return final(valids[0])  # discard smallest incase can lose
            else:  # no hearts
                if following:
                    # Safe to gamble and try to win the trick.
                    # discard greatest
                    return final(valids[-1])
                else:
                    # not following therefore will auto lose, discard greatest
                    if Hvalids:  # safe to break hearts
                        return final(arrange_big(Hvalids)[-1])
                    return final(valids[-1])
        else:
            # if lead
            if heartratio > HRATIO and Hvalids:
                return final(arrange_big(Hvalids)[0]) # Break with smallestH
            else:
                for card in valids[::-1]:
                    # return safe greatest card.
                    if card[1] not in tot_absent:
                        return final(card)
                        # Earlygame, unlikely to gain points by winning trick
                return final(valids[0])  # someone may win
    # End of Early game strategy

    # For mid and late game, they share common logic sets and
    # each have unique strategies.
    # Filters consist of :
    # QSstrategy -> Mid/Lategame unique strategy -> general strategy.
    # if any situation falls under any of the strategies, rightful card will
    # be returned.
    # A card is gurateed to be returned at general strategy (last) filter.

    # Universal QS strategy for mid/late game.
    if QSactive:
        if 'QS' in curr_trick:
            if attempt_lose():
                return attempt_lose()
            # avoid QS

        if not IhaveQS:
            # someone is holding QS.
            for danger in ['AS', 'KS']:
                if danger in valids and len(valids) > 1:
                    valids.remove(danger)
                    req_check = True
            # Completely remove chance of using AS KS.

            # Need to save small QS for safety or make Spade void.
            if len(valids) > 1 and Svalids:
                for card in arrange_big(Svalids)[::-1]:
                    if cardnum(card) < cardnum('QS'):
                        # anything smaller than QS
                        valids.remove(card)
                        req_check = True  # need to update canlose/win
                        break  # only one
                        # Remove for keeping.
        else:
            # if player holding QS
            if curr_trick and 'QS' in valids and \
               (curr_trick[0][1] != 'S' or cardnum(curr_trick[0]) > cardnum('QS')):
                return final('QS')  # return QS only if safe.
            elif len(valids) > 1 and 'QS' in valids:
                # Not safe condition to use QS
                valids.remove('QS')  # Avoid QS usage.
                req_check = True  # need to update canlose/win

    # Compile data to check for anyone trying to shoot the moon.
    noscoreplayer = []
    for x in range(3):
        if allscore[x] == 0:
            noscoreplayer.append(x)

    # Mid/Late game shared shoot the moon stratregy
    if len(noscoreplayer) == 1 and noscoreplayer[0] == player:
        if allscore[player] >= 3 and Hincurr:
            # very little possibles, easy moon shot
            if attempt_win():
                return attempt_win()  # win with efficiency

    elif len(noscoreplayer) == 1 and noscoreplayer[0] != player:
        # someone is trying to shoot the moon.
        # save and avoid using hearts
        if Cvalids or Dvalids or Svalids:
            for card in Hvalids:
                if len(valids) > 1:
                    valids.remove(card)
                    req_check = True

    # General Strategy
    if curr_trick:
        # not leading
        if following:
            # following the lead suit
            if not Hincurr:
                # no H in current trick
                nxtplayers = []
                for x in range(player+1, 4):
                    nxtplayers.append(x)

                safe = True
                for enemy in nxtplayers:
                    if all_absent[enemy] != []:
                        safe = False
                        break

                if safe:
                    if attempt_win():
                        return attempt_win()
                # only one more player playing after me, chances low.
            if attempt_lose():
                return attempt_lose()
            if guranteed_win(curr_trick[0], hand):
                return final(valids[-1])
            return final(valids[0])
        else:
            # not following lead suit
            if priority != 0:
                # not following the lead suit
                return final(arrange_big(valbysuit[priority-1])[-1])
            else:
                if Hvalids:
                    return final(arrange_big(Hvalids)[-1])
                return final(valids[-1])
    else:
        # leading
        if priority != 0:
            return final(arrange_big(valbysuit[priority-1])[0])
            # already filtered and guranteed someone will have that suit
        else:
            for card in valids:
                if card[1] not in str_absent:
                    return final(card)
            return final(valids[0])


def predict_score(hand):
    '''
    hand = starting hand

    take account of how easy you can win a trick
    variety of houses

    Data was collected with sample size of 40,000
    It was divided into total number of cards per suit and
    total number of big cards per suit.

    Average score per criteria was collected by limiting above data in Excel.

    Average score for the data was 2.59.
    True average score was collected with sample size of 1,000,000.
    This was 3.08.
    Using these two, shift was calculated to achive more accurate predictions.

    Because my play() funtion has strategy of trying to score during Late game
    if current score is below prediction, trying to match the score,
    skewing the data slightly is benificial overall

    Optimal skew was found by manually testing each skew for sample size of
    100,000. The skewness of # had greatest accuracy of # percent.

    Upon testing with sample size of 1,000,000,
    82 percent accuracy was achieved.

    Therefore, the following function predicts score with accuracy of 82%.
    Sample size is big enough, as I expect this AI to run less than
    300,000 times, therefore anormalies can be ignored and data can be
    considered normally distributed.

    Therefore, this function predicts score with # percent accuracy.
    '''

    Hcount = [x for x in hand if x[1] == 'H']
    Ccount = [x for x in hand if x[1] == 'C']
    Dcount = [x for x in hand if x[1] == 'D']
    Scount = [x for x in hand if x[1] == 'S']
    Hbig = [x for x in Hcount if cardnum(x) > 6]
    Cbig = [x for x in Ccount if cardnum(x) > 6]
    Dbig = [x for x in Dcount if cardnum(x) > 6]
    Sbig = [x for x in Scount if cardnum(x) > 6]
    nonH = [len(Ccount), len(Dcount), len(Scount)]
    Hc = len(Hcount)
    Cc = len(Ccount)
    Dc = len(Dcount)
    Sc = len(Scount)
    HBc = len(Hbig)
    CBc = len(Cbig)
    DBc = len(Dbig)
    SBc = len(Sbig)

    DATA_AVG = 2.59  # Average from collected data
    TRUE_AVG = 3.49  # Real data
    # Due to implementation of trying to get more scores
    # for bonus rounds in late game strat
    SHIFT = TRUE_AVG - DATA_AVG

    # Following results were extracted from data collected.
    if HBc > 4:
        return int(5.23+SHIFT)
    elif DBc > 4:
        return int(3.64+SHIFT)
    elif CBc > 4 or SBc > 4:
        return int(1.92+SHIFT)

    if Dc > 6:
        return int(1.08+SHIFT)
    elif Sc > 6:
        return int(1.39+SHIFT)
    elif Cc > 6:
        return int(2.05+SHIFT)
    elif Hc > 6:
        return int(3.13+SHIFT)

    if Hc < 2:
        return int(2.2+SHIFT)

    return 0


def get_winner_score(trick, round_id, deck_top):
    playorder = [0, 1, 2, 3]
    if round_id < 4:
        winner = trick_winner(trick, playorder, trump=deck_top[0])[1][0]
    else:
        winner = trick_winner(trick, playorder)[1][0]

    score = 0
    for card in trick:
        if 'H' in card:
            score += 1
        if card == 'QS':
            score += 13

    return(winner, score)

