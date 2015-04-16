'''
Project Rubick
3D Cryptography Program
Inspired by Enigma
Author : Bermuda
Web    : imnotbermuda.com
'''

import numpy as np
import time
import math

TIMELIST = time.gmtime()
YEAR = TIMELIST[0]
MONTH = TIMELIST[1]
DATE = TIMELIST[2]
DOY = TIMELIST[7] #Day of Year

msg = ']}[A];]&[m]!]W]IQ-QtQ5]uQt]uQjQ(QH]uQ9]oQ.Q/]!QTQ*]n]!Q.]!]b]v[~S2S.S9ScSqSeSuSlSUSuSbAj]T]^[S;t'

key = 1658# 4-digit internal key, incase breached. not start or entirely zero.
count = 'qwert5678yulzxcvbnm,./;[]QWER^&*TYUI}!@#OPASDF34GHJKLZXCVBNM<>?:{$iopas12dfghjk()_+-=90`~' #89bit number
print('HEX : {}'.format(len(count)))

####################################################

if len(str(DATE)) == 1:
	strDATE = '0'+str(DATE)
else:
	strDATE = str(DATE)

if len(str(MONTH)) == 1:
	strMONTH = '0'+str(MONTH)
else:
	strMONTH = str(MONTH)

if len(str(DOY)) == 1:
	strDOY = '00'+str(DOY)
elif len(str(DOY)) == 2:
	strDOY = '0'+str(DOY)
else:
	strDOY = str(DOY)
# TODO :
# There is more elegant way to do this than if-else bomb!

strKEY = str(key)

for num in strKEY, strDOY, strMONTH, strDATE: #Shuffle countkey
	num = int(num)
	count = count[num:] + count[:num]

print('KEY : '+ count)

def recount(num):
	d = ''
	if type(num)==int:
		while num//89 > 0:
			d += count[num//89]
			num -= 89*(num//89)
		return d + count[num%89]
	else:
		print('Error. Integer only.')

####################################################################################

enclist = []

while msg:
	a = msg[0]
	b = msg[1]
	msg = msg[2:]
	enclist.append(count.index(a)*89 + count.index(b))
#print(enclist)
#print(len(enclist))

####################################################################################

x = math.ceil(math.pow(len(enclist),1/3.0))#always round up the squareroot
y = x+int( str(key)[ int( strDATE[0] ) ] )%3#+DATE%2
z = x-int( str(key)[ int( strDATE[0] ) ] )%3
SIZE = x*y*z
if SIZE - y*z >= len(enclist):
	x -=1
#print(len(msgList))
#print(x)
#print(x,y,z)

####################################################################################

msgMtrx = np.zeros((x,z,y))
msgMtrx = np.rot90(msgMtrx)

tx ,ty, tz, tk = 0, 0, 0, 0
#print(msgMtrx)

for element in enclist:
	msgMtrx[tx][tz][ty] = element
	#print('appended to {} {} {}'.format(tx,ty,tz))

	if ty < y-1:
		ty+=1
	elif tz < x-1:
		ty = 0
		tz += 1
	else:
		tx += 1
		ty, tz = 0, 0



msgMtrx = np.roll(msgMtrx,-int(strKEY[3]))
msgMtrx = np.rot90(msgMtrx, -1)
msgMtrx = np.roll(msgMtrx,-z)
#print(msgMtrx)

####################################################################################

tx ,ty, tz, tk = 0, 0, 0, 0
tempMtrx = np.zeros((x,y,z))

while tx < x:
	#if(msgMtrx[tx][ty][tz] != 0):
	tempMtrx[tx][ty][tz] = msgMtrx[tx][tz][ty] - int(YEAR/int(str(key)[tk]))
	#else:
	#	tempMtrx[tx][tz][ty] = msgMtrx[tx][ty][tz] + int(YEAR/int(str(key)[tk]))
	#print('appended to {} {} {}'.format(tx,ty,tz))
	# If wish to add differnt value for 

	if tz < z-1:
		tz+=1
	elif ty < y-1:
		tz = 0
		ty += 1
	else:
		tx += 1
		ty, tz = 0, 0

	if tk < 3:
		tk+=1
	else:
		tk=0

####################################################################################

msgMtrx = tempMtrx
print(msgMtrx)

finalmsg=''
tk = 0

for twoD in msgMtrx:
	for oneD in twoD:
		for element in oneD:
			if element != 0:
				finalmsg+=chr( int(element) - (tk%2)*int(DOY/DATE) )
			tk+=1
print(finalmsg)
