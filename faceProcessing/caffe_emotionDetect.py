_author__ = 'pujandave'

import numpy as np
import sys
import cv2
import cv
import os
import threading
import time


# Make sure that caffe is on the python path:
caffe_root = '/home/pujandave/Documents/caffe-master/'  # this file is expected to be in {caffe_root}/examples
sys.path.insert(0, caffe_root + 'python')
sys.path.append("/usr/lib/python2.7/dist-packages/")

import pygame
import caffe

MODEL_FILE = '/home/pujandave/Documents/caffe-master/models/finetune_facial/deploy.prototxt'
PRETRAINED = '/home/pujandave/Documents/caffe-master/models/finetune_facial/finetune_facial_iter_12600.caffemodel'
IMAGE_FILE = '/home/pujandave/Documents/caffe-master/examples/images/face.png'

#caffe.set_phase_test()
caffe.set_mode_cpu()
net = caffe.Classifier(MODEL_FILE, PRETRAINED,
                       mean=np.load(caffe_root + 'python/caffe/imagenet/ilsvrc_2012_mean.npy').mean(1).mean(1),
                       channel_swap=(2,1,0),
                       raw_scale=255,
                       image_dims=(256, 256))


cap = cv2.VideoCapture(0)
textColor = (0, 0, 255)
str1=' '

ret, m_f = cap.read()

cnt=0;
main_frame = fr
while(True):
    cnt = cnt+1
    if cnt%20==0:
        if true:
            s='./FaceCropTool'+' '+'test.jpg'+' '+'test.jpg'
            os.system(s)
            iimg = cv2.imread('test.jpg')
            gray = cv2.cvtColor(iimg, cv2.COLOR_BGR2GRAY)
            equ = cv2.equalizeHist(gray)
            cv2.imwrite('test.jpg',equ)
            fw=open('face.txt','r')
            fwlen=fw.readline()
            fw.close()
            print fwlen
            if fwlen.strip()!='':
                input_image = caffe.io.load_image('/home/pujandave/Documents/FacialEmotion/linux64_c++_v1.2/test.jpg')
                prediction = net.predict([input_image])
                print prediction
                if prediction[0].argmax()==0:
                    str1='Neutral'
                elif prediction[0].argmax()==1:
                    str1='Angry'
                elif prediction[0].argmax()==2:
                    str1='Disgust'
                elif prediction[0].argmax()==3:
                    str1='Fear'
                elif prediction[0].argmax()==4:
                    str1='Happy'
                elif prediction[0].argmax()==5:
                    str1='Sadness'
                elif prediction[0].argmax()==6:
                    str1='Surprise'
                elif prediction[0].argmax()==7:
                    str1='Contempt'
                else:
                    str1=' '
        
            cv2.putText(main_frame, str1, (50, 50),cv2.FONT_HERSHEY_PLAIN, 3.0, textColor,4,cv2.CV_AA)

        cv2.imshow('frame2',main_frame)

        if cv2.waitKey(10) & 0xFF == ord('q'):
            self.running = False
            break

cap.release()
cv2.destroyAllWindows()


