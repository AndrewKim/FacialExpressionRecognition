__author__ = 'pujandave'

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


while(True):
    ret, frame = camera.read()
    m_f = frame
    cv2.imshow('frame', frame)
    cv2.imwrite('test.jpg',main_frame)
    if cv2.waitKey(10) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()















