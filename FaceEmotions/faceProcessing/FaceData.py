__author__ = 'wangchen'
import os
import glob

def faceCrop(imagePattern):
    imgList=glob.glob(imagePattern)
    if len(imgList)<=0:
        print 'No Images Found'
    for img in imgList:
        s="./TestDetector "+img+" "+img
        os.system(s)

f=open('label.txt','w')
f.truncate()
f.close()
f=open('com.txt','w')
f.truncate()
f.close()
faceCrop('jaffe/*.tiff')