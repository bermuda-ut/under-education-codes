###############################################################
#
# Random game player -- generates a shuffled pack of cards and
# deals the cards to the four players, then plays the player
# against itself, optionally in bonus mode
#
# NOTE: relies on an implementation of the following function
# in `program.py':
#
#   get_winner_score(trick, round_id, deck_top):
#
# which takes a single trick `trick' (a tuple), the round_id
# (an int) and `deck_top' (a list of strings, as per `score_game')
# and returns a 2-tuple:
#
#   (ID, trick_score)
#
# where `ID' is the index of the winnder of the trick (relative
# to the order of play in the trick, and *not* the global player IDs)
# and `trick_score' is the score for the current trick (to be
# allocate to the winning player). For example:
#
# >>> get_winner_score(('2S', 'AS', 'QS', '7D'), 2, ['2D', '3S'])
# (3, 13)
# >>> get_winner_score(('2S', 'AS', 'QS', '7D'), 4, ['2D', '3S', 'JS'])
# (1, 13)
#
#
# Author: Tim Baldwin:
#
# Version 1
#
# Date: 22/5/2015
#
#
###############################################################
#
# Further edited by Max Lee
# 11/7/2015
#
###############################################################

import itertools, random, time
from collections import defaultdict

import ai as program
import copy
import csv

file = open("./data.csv", mode='w')
csvfile = csv.writer(file, delimiter=',', lineterminator='\n')

VALUES = 'AKQJ098765432'
SUITS = 'SHDC'

n = 50

HANDSIZE = 10

PLAYERS = 4

PHASE_ONE_TRICKS = 3

TOTAL_TRICKS = 13

QSbombed = 0
QScount = 0

faroff = 0
acceptable = 0
perfect = 0

tscr = 0
avscr = 0

starthandz = []
scorez = []


def generate_random_game(rounds, bonus=False, verbatim=True):
    assert 0 < rounds <= TOTAL_TRICKS
    deck = [val+suit for val,suit in itertools.product(VALUES, SUITS)]
    random.shuffle(deck)
    players = [[], [], [], []]
    playergains = copy.deepcopy(players)
    global QScount
    global QSbombed

    global faroff
    global acceptable
    global perfect
    global tscr
    global avscr

    global starthandz
    global scorez

    for i in range(HANDSIZE):
        for j in range(PLAYERS):
            players[j].append(deck.pop())
    prev_tricks = []
    deck_top = [deck.pop()]

    if bonus:
        pred_scores = []
        player_data = []
        for i in range(PLAYERS):
            score = program.predict_score(players[i])
            pred_scores.append(score)
            player_data.append(score)
        pred_scores = tuple(pred_scores)

    prev_winner = 0
    QSstart = None
    for round_id in range(rounds):
        if round_id == 0:
            for hand in players:
                starthandz.append(copy.copy(hand))
            for p in range(4):
                for card in players[p]:
                    if card == 'QS':
                        QSstart = p
                        break
        curr_trick = []
        for j in range(PLAYERS):
            player_id = (prev_winner + j) % PLAYERS
            if bonus:
                card, player_data[player_id] = program.play(curr_trick, players[player_id], prev_tricks, deck_top, player_data=player_data[player_id])
            else:
                card = program.play(curr_trick, players[player_id], prev_tricks, deck_top, suppress_player_data=True)
            curr_trick.append(card)
            playergains[player_id].append(card)
            #print("card = {}".format(card))
            players[player_id].remove(card)
        curr_trick = tuple(curr_trick)
        prev_tricks.append(curr_trick)
        curr_winner, _score = program.get_winner_score(curr_trick, round_id, deck_top)
        prev_winner = (curr_winner + prev_winner) % PLAYERS
        if round_id < PHASE_ONE_TRICKS:
            for j in range(PLAYERS):
                if j == prev_winner:
                    players[j].append(deck_top[round_id])
                else:
                    players[j].append(deck.pop())
            if deck:
                deck_top.append(deck.pop())

    scores = program.score_game(prev_tricks, deck_top)

    if verbatim:
        if bonus:
            #print("PREDICTED SCORES: {}".format(pred_scores))
            #print("ORIGINAL SCORES: {}".format(scores))
            gains = []
            rev_scores = []
            for i in range(PLAYERS):
                diff = scores[i] - pred_scores[i]
                gains.append(diff)
                diff = abs(diff)
                if diff == 0:
                    rev_scores.append(scores[i] - 10)
                    perfect += 1
                elif 1 <= diff <= 2:
                    rev_scores.append(scores[i] - 2)
                    acceptable += 1
                elif 3 <= diff <= 4:
                    rev_scores.append(scores[i])
                    acceptable += 1
                elif 5 <= diff <= 6:
                    rev_scores.append(scores[i] + 2)
                    faroff += 1
                elif 7 <= diff <= 8:
                    rev_scores.append(scores[i] + 4)
                    faroff += 1
                else:
                    rev_scores.append(scores[i] + 10)
                    faroff += 1
            scores = tuple(rev_scores)
            #print("ADJUSTED SCORES: {}".format(scores))
            #print("DIFFERENCE SCORES: {}".format(tuple(gains)))
        else:
            #print("SCORES: {}".format(scores))
            for scr in scores:
                scorez.append(scr)
            temp = 0
            for score in scores:
                tscr += score
                temp += score
            avscr = (avscr + temp/4)/2
            if not QSstart:
                QScount += 1
            for p in range(4):
                for card in playergains[p]:
                    if card == 'QS':
                        if p != QSstart:
                            QSbombed += 1
                        break


print("Test started....")
start = time.time()
perc = [int(x) for x in range(n) if x%(n*0.05)==0]
#print(perc)
for x in range(n):
    generate_random_game(13, bonus=False)
    if x in perc:
        print("Status... {:2}%".format(int(x/n*100)))
end = time.time()

print("""
AVGSCR = {}
True AVG = {}""".format(tscr/(n*4), avscr))
if QScount > 0:
    print("QSbombed = {}, QSpresents = {}".format(QSbombed, QScount))
    print("QS bomb chance = {}%".format(int(100*QSbombed/QScount)))
if (acceptable+perfect+faroff) != 0:
    finalratio = int(100*(acceptable+perfect)/(acceptable+perfect+faroff))
    print("""
faroff : {}
acceptable : {}
perfect : {}
good/total : {}%""".format(faroff, acceptable, perfect, finalratio))


SUIT = ['C', 'H', 'D', 'S']
CNUM = [str(x) for x in range(2, 10)] + ['0', 'J', 'Q', 'K', 'A']
CARDS = []

# creation of deck
for st in SUIT:
    for num in CNUM:
        CARDS.append(num+st)

CARDS+=['scr']

csvfile.writerow(CARDS)
for x in range(len(scorez)):
    scr = scorez[x]
    hnd = starthandz[x]
    row = [0]*52
    for card in hnd:
        idx = CARDS.index(card)
        row[idx] = 1
    csvfile.writerow(row+[scr])

file.close()

print("Elapsed Time : {}".format(end-start))
