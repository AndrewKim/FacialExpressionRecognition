__author__ = 'wangchen'
import os
import glob

def faceCrop(imagePattern):
    imgList=glob.glob(imagePattern)
    if len(imgList)<=0:
        print 'No Images Found'
    for img in imgList:
        s="./FacePreprocessing "+img+" "+img
        os.system(s)
faceCrop('jaffe/*.tiff')