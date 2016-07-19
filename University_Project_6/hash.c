/*=============================================================================
#     FileName: hash.c
#         Desc: assignment 2
#       Author: Max Lee
#    StudentID: 719577
#        Email: hoso1312@gmail.com
#     HomePage: imnotbermuda.com
#      Version: 0.0.1
#   LastChange: 2016-05-20 01:44:08
=============================================================================*/
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXSTRLEN 256

/* Used as the second hashing function on double hash */
unsigned int linear_probe(void *e, unsigned int size) {
    (void) e;
    (void) size;
    return 1;
    // Hardcore version:
    // include inttypes.h
    // return ((uintptr_t) e % size) % size;
    //
    // Fun fact, our size is PRIME NUMBER so ANYTHING is relatively prime
    // Therefore, any value is effective!
}

/* Very simple hash */
unsigned int worst_hash(void *e, unsigned int size) {
    (void) e;
    (void) size;
    return 0;
}

/* Basic numeric hash function */
unsigned int num_hash(long n, unsigned int size) {
    return n % size;
}

/* Bad hash function */
unsigned int bad_hash(char *key, unsigned int size) {
    static unsigned int constant = 0;
    if(constant == 0) constant = rand() % size;

    return (constant * key[0]) % size;
}

/* Universal hash function as described in Dasgupta et al 1.5.2 */
unsigned int universal_hash(unsigned char *key, unsigned int size) {
    unsigned int sum = 0,
                 i   = 0;
    static int   *randnums = NULL;

    // generate rand
    // Would love to use getRandArray() in hash.c but cannot edit header file =(
    if(randnums == NULL) {
        randnums = malloc(sizeof(*randnums) * MAXSTRLEN);
        for(i = 0; i < MAXSTRLEN; i++)
            randnums[i] = rand() % size;
    }

    // Textbook style universal hash
    for(i = 0; i < strlen((char*)key); i++)
        sum += key[i] * randnums[i];

    return sum % size;
}

/*
            * SQUINT YOUR EYES TO MEET OUR LORD ALISTAR MOFFAT *
MMMMMMMMMMMMMMMMMMMMMMMMMMMNMMNNNMNMMMNNMMNMMNMNDDDNNNMMMMMMMNNMNMMMNMNNMNNMNMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNNMMMND8DDD88ONONNM8NMMNNMMMMMNMMMMMMMNNNNNNNMNNN
MMMMMMMMMMMMMMMMMMMMMMMMMNN8$?II7I77?IZ8$Z8OODO8D88DZM8ONMMNNMMNMMMNNNNNNDNNNNNN
MMMMMMMMMMMMMMMMMMMMMNNNN77$$$Z$?IZI++?+++7ZI$8D8OZ88D8DNNDNNNMNMMNNNNDNNNNNNNDD
MMMMMMMMMMMMMMMMMMMMMNMIZZ?$I$7I?+II?77$Z$II7$O8O8O8D8O7ZNDNNNMMMNNNNNNDNNMMDNDD
MMMMMMMMMMMMMMMMMMMMNDOZIZ?I?+=???I$7$$7I?II?7Z7D8DDOZ8OOZ8ODMMMMMNMNNNNMMMNMNNN
MMMMMMMMNNNNNMMMMMND$?7I7+I=+?+?77$I+?7ZOZI7$ZOO8O88OO$$777ZMNMMMMMNMMNMMMMMMNMM
MMMMMMMMNMNNNMNNMMN$III$+I??+++~~~~~====+?+?II7$ZOOOOOZZZ777ONMMMMMMMMMMMMMMMMMM
NMMMMMMMNNNNNNNNND$$$8$7O$7I??$ZI?+????II7????????$ZZZ7$77$DDNMNNNMMMMMMMMMMMMMM
NNNNMMMMNNNNNDNMO7IIZ$I$$ZII77II?+++===========~~=+IZ$7$7$$8MNNNNNNMMMNNMMMMNMNM
NNNNNNNNNNNNNNNNZZ$I?I$77?==~~~~::::::::::::::~~~~=+7ZZ7$$ZONNMNNMMMMNNNNNMMNNNM
MMNMDDNMNMNMMM8Z$7$=???+=:::::::::,,,,::::::::::::~=+?$OZ$8DMNNNMMNNNNNNNNNNNNMM
NNMD8DDDMMMMNN$ZI+II+??=~:::::::,,,:,,,,,,::,::::::~=+I$ZOZDNNNNNMNNNNNNDDNNNNNN
MMNDDDDDDNMMNZ7?$I?+~=~~:::::::,,,,,,,::::::::::::~~=++IZZONNNNNMNMNMNNNMNDNMNNN
MNNDN8DNNMMMZ$Z7?I==+=~~::::::::::,,::::::::~~~~~~===++?Z8NNNNMNNMMNNDDNNNDNNNNN
MMMNMN8NDDMMN$$I?I+==~:::::::::::::::~:::~~~~:~=~=+++++?IODDDNNNNNNMMMNMDMMMMNDM
MMMMMNDNDMMNM$7$7I?===~~:::~~~::::~~~:::~~~~~~====+++???IZNNNNNNDMNNNDNMNMMMMMNM
MMMMMNMMDNMMN8$Z7I++?==~:~~~~~~~::~::::::~~~~===~==++??I7$NMNNNMNNNDNNMNNMMMNNNN
MMMMMMMMMNMMNO7$I?=+++==~~~~::::::~:::,:::~~~=~~~~~=+???I$DNNMMNMDNMNMMMMMMMMNNN
NMMNNNMMMMMM8Z8$I?=+++==~~~~:::::::::::~~~~~~~~~~==+++++?$8NNMMNMNMMNMMMMMMMNNND
NMMNNNMMMMMMDDZZI++??+==~~~~:,,,::::::~~==+====++++++++=+$DMNMMMMMMMNMMMMMMMMMNN
NNMNMMNMMMMMN8Z87I++?===~~~~:~~+??+???+=+?+++?+???I7$$$I?$NMMMMMMMMMMMMMMMMMMMMM
DNMMMMNMMMMMM?=+$8$I++~=~~~~=?II7ZZ88D8ZZZI??I$$O8DDD88OZOMMMMMMMMMMMMMMMMMMMMMM
MNMMMMMMMNMN$?Z7+=ZO$+=~~:~=I?++$88O88ZOOOI+?I888OOO8OZZO$MNMMMMMMMMMMMMMMMMMMMM
MMMMMMNNNNNNZ7$+?++ZI?MI~:~=?$Z$7ZO$I$$$$Z+~=?MN$Z8ZNZDD$MMNNMMMMMMMMMMMMMMMMMMM
MMMMNNNNNNMMN===?$?+?+++=$I~=++?==++?7$$IIOD$$NZ$$?7II$OINMMNMMMMMMMMMMMMMMMMMMM
NNMMMMMNNNNNM?~?7I~=??++=~~:==~=+?77$77I+$7::~I7$$$7777I?ZNDMMMMMMMMMMMMMMMMMMMM
MNMMMMMMMMNMN8+~+=~=????+=~:~=~~=+???+==++~~::=7??77I?++IZ?$MMMNMMMMMMMMMMMMMMMM
NNNNMMMMMMMMNN=+~=++????++==~~::~======~?~::,:~+7?++=++?7$MNMMNNNMMNMNMMMMMMMMMM
NNNNMMMMMMNNNNM?=~~??????+++=~~~====+?II~~~:,:~=77?????I$ZNMMMMMMMMNNNNMMMMMMMMM
MNNNNMNMMNNMNNNN?+=+?????++++=~~===+I7?=::::::~=?+7I???I$8NMMMNMMMMNNNMMMMNMMMMM
NMNNNNNNMMNMMMNMNMI??????++++====+?7ZI+IODI++?I$OOO$7II7ZNMMMNMMNNMNMMNNNNNNNMMM
NMNNNNNNNNNNMMMMNNDMO?+++??+++++??7O?+==I77$7ZODZ$$O$77$ZMMMNMNMMMNNMNNNNNNMMMMM
NDDNNDMNNNNNMNNN8DDNN7??+??++???7$$I?++++?7ZO88O$$7$Z$ZZ8NNNMMNMMNDDNNNNNMNMMMMM
DDDDDDNNNNDDDD88NDMNNO??????+??I$$????+???7$$$$$$7$$ZZZZMNNNNNMNMNDDNMNNMNMMMMMM
NDNNDNDDDDDNDNDDDNNNMNII???II??I7??77I++???+??I77$Z$$$$ZMNDNNNNDDDNMNNMMMMMMMMMM
NNNNNNNNNDNDDNDNNDNDNMII???????I??788OZOZ$$$ZOOODDDZZ7$Z$7I????I$ONMNNMMMMMMNMNN
NNNNNNNNDDDDDMNDMNNMNNIIIII?I?I???7I+===~~===~==777Z$7O$$$7$77II???????$NMNMNMMN
NNNNNMMDDDNDMMMNNNNNNO7?IIIIIII??II?=+?+?+++++I77$$$ZZO$$$$$$777I777IIIII??+7OMM
DNNNMNNMDNNDNNNNNNDM7I$IIII77II7III?+++?7$ZO88D8Z$77ZODO$$$Z$$ZZ$77777III???+???
NNNNDNDNDNNNNNNNNZ??IID?III7777III?++==++??II7777777ZMD8Z$$$$Z$$$$77$$$7II7777II
NNNNDNNNNNND8O$I+I??IIO$IIII77$$II+++==+===+?????I7Z7DD8Z$$$$ZZ$$$$77777III7IIII
NNNDDNMNDDMNN$+????IIIZD?II77777ZZI?++=+++?II?IIII$$$ONDZ$$$$ZZ$$$$$77$7777IIIII
NNNDZ88NDNN8+???IIIIII$NIIII777$$$Z7???????III77$$Z$$ONDZ$$$$ZZZ$$$$$$$7777IIIII
DDDDNN8DDN?=+??IIIIIII$87IIII77$$Z$Z$$7$$$ZZZZZO8OZZZONDOZ$$ZZZZ$$$$$$$7777IIIII
NDNDNN$I=+????IIIIIIII7ZDIIIII7$$77$$$ZOO8O8OOOZZZZZZODDOZ$$$$ZZZ$$$$$7777777III
NDDDZI=?II??I?777IIIII7$D8IIII77$7II7$$ZOOOOOOZOOZZZZZD8OZ$$$$$$$$$$$$7777777II7
DDD?=+??+??III$77IIIIII7OD$?II77777777$$ZZOOOZZO8OOZZO88OZZ$$$$$$$$$$$7777777I77
D?=+=+?7$7???I7$777IIII7Z8O7?III777$7$77$ZZOOOO8OOOOZ8O8OZZ$$$$$$Z$$$$7$$$77I777
+==+I++?7I?+?I7Z77I7III7ZOOZ7IIII777$$77$$OOOOOOOOOZZ8ZOOZ$$$$77$$ZZ$$7$$$777777
+=+I?+++I7II$7IZ$7III7I7$OOZ77I7777$$$$ZZOOOOOZZZZZZZ8ZZZ7Z$$$$7$ZZZ$IIZ$$7I7777
+7++++++??7III7ZZ$77III77Z8$77II77777$$ZZOOOOZZZZZ$$ZOZ$ZZ$7$$$7$ZZ$$7777$777$$7
+++++?+++?I7I7$7ZZ$7IIII7$OZI?III77777$ZZZOZZ$$$$7778OZZ$Z7$$$7$OZ7777ZI7$77$$77
++++++?+++??$7$7ZZZ$7III7$ZZ$IIIIIII77$ZZOOZ$$77III$Z$O$Z$I:ZD$$7777777$7$77Z$77
+++?++???+??I$$ZOZZ$777I77ZZ$OIII?III7$$ZZZ$7$7IIIIZ$ZO7O$DDNO77II777$$77$$Z$777
++?I????I????IZOZZZZ$7IIII$Z$OO$II7III$$$$$7I7III?$$$$OI=:::::::~+777Z$$$Z$O77$$
?+??II???II???IZ7OZZ$77III7Z7OZO$77III77$$77III??IZZZ===::==??++?II77ZZ$ZZ$87777
??+?IIII?I7IIII7D7OZ$$7III7$7Z$ZOZ7II777777I????IZI~:,=~+77$OOZOO$$7??$$OZ$D$77$
?????I77I?777III7Z8OZ$7IIII$78ZZZ$$777$$7777I???:::::~::::::::::::,,IDMZ$ZZD$$$Z
7I????II77I777II77O8D8DDNND7ZZ$$ZZOZ7777III7I+:~:::::::,::::,,:,,~:~:::~?8ZDZZZ7
I7II???I7$I777$$$$77ZZZZO8DI7$$Z$$ZZZ777II7+=~::~:::::::::::::~~~=++====+?+NZOZ8
II777I?II7ZI77$$Z$$$$ZZZZO87ZZ$Z$$ZZZZ7I?~=~::::::::~~++=+++????III???++?I+=OZD$
7II7$7III77Z7777$$Z$$78OO888D8ZZ77ZZ$Z?~=~:::~:::::::::~?$DDMDOZ8NOZZZ$$Z$77D$$$
II7I7777I777$77$$$ZZ$$$8888888OOODOZ$=~~~~~~~:~:::,,,,::::~~~~==++++IZMMNZNN7$8N
$7I$777$7777$Z7$$$$ZZ$$$888888OOZZN=~~::::::::~:~:::,:::::~~~~~~~:~:~+??778$ZD8O
77$77$77Z7777Z7$$$ZZZZ$7OO8OO8OOZ7=~~~:~::::~~::::::~~======~~~~~~~~~+????OZN8ZZ
II77$7I$$7$$$$O7$$$ZZZ$$7OOOOO8I=~=::::::::::::~~===+++=++++++=+++??I77II7INOZZZ
III777$$77$$$$$8$$$ZZZZ$77OOOZ=~~~~~::~~::::~~~~~::~~~=+I$888OOZOOZ$$$$$Z$Z8ZZZZ
II?II??III77O8Z$Z$$$ZOOZ$7$O===~~~~~~~:::~~~::::~::,,::~~~~~~~=+?II$8OOOOODZZ$$$
77$$ZZOZZ$$77IO8ZO$$ZOOO$$$+=~~~~~~~::~~~:::~~~~==~~~=++++===+++????OOOOONOZZZZZ
I?IIIIII77$ZZ$$7D8OZ$OOOZ$?==~~~~~~~~~~~=~=+===+=~==+???+??IIIIIII7$88OOONZZZZZZ
I7IIIIIII?II77$$$$NOZZ88O$+===~~~~~==+++==+=========++?7Z8DDDDD8Z$O88888O8Z$$ZOO
III???IIIIIIIII7$$Z7OZ88O=+===============~~~~~~~~====+???II7$ZOZ$$O88888NOZ7777
IIIIIIIIIIIIIII777$$$Z$8===+===+=======+++++=+==~~===+?IIII7$$$$Z$$Z88888DOOOOOO
??II?????IIII?????II7$OZ+=+=+==========+??+???????++I7$77$$$Z$I$I$Z788888N8OOOOO
?????II7I77777$$ZZZO8OO??++++++++===~=+?++????IIII77$Z$ZZ77$$?7ZOZ$$$$888D8OZZZZ
7I??+???II777777IIII777???++++++??~=+?III7III777$$$$7$Z77$7$$77ZZO77Z$$Z8D8OZZZZ
                               Algorithms are Fun
*/
