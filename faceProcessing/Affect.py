__author__ = 'sunknight'
import os
import cv2
import numpy as np



fw=open('/home/sunknight/EXPRESSION/label.txt','r')
fwlen=fw.readlines()


for img in fwlen:
    print img[0:76]
    s='./FaceCropTool'+' '+img[0:76]+' '+img[0:76]
    print img[0:76]
    os.system(s)
    image = cv2.imread(img[0:76],0)
    equ = cv2.equalizeHist(image)
    cv2.imwrite(img[0:76],equ)