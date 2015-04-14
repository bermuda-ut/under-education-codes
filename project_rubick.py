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

msg = 'the quick brown fox jumps over the lazy dog. THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.'

key = 1658# 4-digit internal key, incase breached. not start or entirely zero.
count = 'qwert5678yulzxcvbnm,./;[]QWER^&*TYUI}!@#OPASDF34GHJKLZXCVBNM<>?:{$iopas12dfghjk()_+-=90`~' #89bit number
print('HEX : {}'.format(len(count)))

#################################################### 

msgList = list(msg)

for nth in range(0,len(msgList)):
	msgList[nth] = ord(msgList[nth]) + (nth%2)*int(DOY/DATE)

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

#################################################### UNICODE + ADD DOY

# determine the size of the 3d message matrix
x = math.ceil(math.pow(len(msgList),1/3.0))  # always round up the squareroot
y = x+int( str(key)[ int( strDATE[0] ) ] )%3 # Will-do for now.
z = x-int( str(key)[ int( strDATE[0] ) ] )%3 # Will.do for now
SIZE = x*y*z
if SIZE - y*z > len(msg):
	x -=1
#print(x,y,z)

msgMtrx = np.zeros((x,y,z))

# NOTE : >z ^y inx
tx ,ty, tz = 0, 0, 0
while msgList:
	#print('appended to {} {} {}'.format(tx,ty,tz))
	msgMtrx[tx][ty][tz] = msgList[0]
	del msgList[0]

	if tz < z-1:
		tz+=1
	elif ty < y-1:
		tz = 0
		ty += 1
	else:
		tx += 1
		ty, tz = 0, 0
	
#print('#### Msg matrix ####')
#print(msgMtrx)

#################################################### 3D matrix

tx ,ty, tz, tk = 0, 0, 0, 0
tempMtrx = np.zeros((x,z,y))

while tx < x:
	#if(msgMtrx[tx][ty][tz] != 0):
	tempMtrx[tx][tz][ty] = msgMtrx[tx][ty][tz] + int(YEAR/int(str(key)[tk]))
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

msgMtrx = tempMtrx
#print('#### Inversed ####')
#print(msgMtrx)

################################################### 'inverse' along y=-x graph + key mix

msgMtrx = np.roll(msgMtrx,z)
#print('#### Shuffle1 ####')
#print(msgMtrx)
msgMtrx = np.rot90(msgMtrx)
#print('#### Rotated ####')
#print(msgMtrx)
msgMtrx = np.roll(msgMtrx,int(strKEY[3]))
#print('#### Shuffle2 ####')
#print(msgMtrx)
################################################### Matrix 


finalmsg = ''

zerocount = 0
for twoM in msgMtrx:
	for oneD in twoM:
		for element in oneD:
			finalmsg+=(recount(int(element)))
print(finalmsg) # print encrypted msg