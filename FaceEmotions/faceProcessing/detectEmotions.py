##########################################################################
#------------------------------------------------------------------------#
#--- Author: Pujan Dave                                               ---#
#---         B.S., Computer Engineering                               ---#
#---         University of Illinois at Urbana-Champaign               ---#
#---         Social Robotics Lab, NUS                                 ---#
#---                                                                  ---#
#------------------------------------------------------------------------#
##########################################################################
#                      Emotion Detection                                 #
#                                                                        #
#  This code will use a camera to capture image frames to detect         #
#  face. If a face is detected then it is cropped and then the image is  #
#  passed into a Convolution Neural Network built using Torch            #
##########################################################################

import numpy as np
import sys
sys.path.append("/usr/lib/python2.7/dist-packages/")
import cv2
import os
import re

import time

import pygame

Reader='./speech.sh '

def readsent_online(a):
    sep='\. |\.\" |\,\" |, |\? |\!'  # to separate , ," ."
    s=re.split(sep,a)
    s.reverse()

    while  s:
        sent=s.pop()
        sent=re.sub('\"', '', sent)
	sent=re.sub('\'s','s',sent)
	sent=re.sub('\'ve',' have', sent)
	sent=re.sub('d\'','do ',sent)
	sent=re.sub('\'t','ot',sent)
	sent=re.sub('\'','',sent)
	print sent
	var=Reader+sent
	os.system(var)

cap = cv2.VideoCapture(0)
textColor = (0, 0, 255) #BGR
cnt=0
str1=' '

torchModule = 'qlua /home/sunknight/FaceEmotions/torch_module/rock.lua'
imgStore = '/home/sunknight/FaceEmotions/torch_module/input/test.png'

ret, frame = cap.read()

while(True):

    ret, frame = cap.read()
    cv2.putText(frame, str1, (50, 50),cv2.FONT_HERSHEY_PLAIN, 3.0, textColor,4,cv2.CV_AA)
    cv2.imshow('CameraFeed',frame)
    cnt=cnt+1

    if cnt%10==0:
        start_time = time.time() #############
        cv2.imwrite(imgStore,frame)
        cnt=0
	iimg = cv2.imread(imgStore)
        gray = cv2.cvtColor(iimg, cv2.COLOR_BGR2GRAY)
	cv2.imwrite(imgStore,gray)
        s='cd /home/sunknight/FaceEmotions/faceProcessing && ./FaceCropTool'+' '+imgStore+' '+imgStore
        os.system(s)

        iimg = cv2.imread(imgStore)
        gray = cv2.cvtColor(iimg, cv2.COLOR_BGR2GRAY)
        equ = cv2.equalizeHist(gray)
        cv2.imwrite(imgStore,equ)
        fw=open('/home/sunknight/FaceEmotions/faceProcessing/face.txt','r')
        fwlen=fw.readline()
        fw.close()
        print fwlen
        if fwlen.strip()!='':
            os.system(torchModule)
            fbin = open('/home/sunknight/FaceEmotions/torch_module/output/class_label.bin','rb')
            prediction = fbin.readline()
	    fbin.close()
            print prediction
            if prediction=='0':
                str1="No emotion!"
                textColor = (200,60,320)
            elif prediction=='1':
                str1="Why are you so Angry!"
                textColor = (0,0,255)
            elif prediction=='2':
                str1="That's Disgusting!"
                textColor = (0,80,200)
            elif prediction=='3':
                str1="Don't be afraid!"
                textColor = (0,0,255)
            elif prediction=='4':
                str1="Yeah! Happy!!"
                textColor = (40,100,0)
            elif prediction=='5':
                str1='Why so Sad?'
                textColor = (100,60,320)
            elif prediction=='6':
                str1='Surprise!!'
                textColor = (255,60,0)
            elif prediction=='7':
                str1='Contempt'
                textColor = (0,0,255)
            else:
                print "Can't predict"
                str1=' '
            print str1
            readsent_online(str1)

        print("--- %s seconds ---" % (time.time()-start_time))  

    if cv2.waitKey(10) & 0xFF == ord('q'):
        break

