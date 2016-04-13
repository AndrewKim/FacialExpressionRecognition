__author__ = 'wangchen'
from sklearn import svm
import numpy as np
import textwrap
import cv2
import cv
import os
import glob
def FaceTest(img):
    f=open('rec.txt','w')
    f.truncate()
    f.close()
    s="./DemoProcessing "+img+" "+img
    os.system(s)
    s="./DemoRec "+img
    os.system(s)
    X=[]
    f=open('rec.txt')
    line=f.readline()
    line=line.rstrip()
    parts=line.split('\t')
    numbers=[int(P) for P in parts]
    predictval=clf.predict([numbers])
    if predictval==[1]:
        x = "SAD"
    elif predictval==[2]:
        x = "FEAR"
    elif predictval==[3]:
        x = "ANGRY"
    elif predictval==[4]:
        x = "HAPPY"
    elif predictval==[5]:
        x = "DISAPPOINT"
    elif predictval==[6]:
        x = "SURPRISE"
    elif predictval==[7]:
        x = "NEUTRAL"
    else:
        x = "error"
    print(x)
    return x



def FaceTrain():
    X=[]
    test=0
    count=len(open('com.txt','rU').readlines())
    F=open('com.txt')
    for i in range(1,count+1):
        line=F.readline()
        line=line.rstrip()
        parts=line.split('\t')
        numbers=[int(P) for P in parts]
        X.append(numbers)
    print len(X)
    Y=[]
    count=len(open('label.txt','rU').readlines())
    F=open('label.txt')
    for i in range(1,count+1):
        line=F.readline()
        line=line.rstrip()
        parts=line.split('\t')
        label=[int(P) for P in parts]
        Y.append(label)
    print len(Y)
    print X[1]

    #clf = svm.SVC()
    clf.fit(X[0:200], np.ravel(Y[0:200]))
    for i in range(201,211):
        if clf.predict([X[i]])==np.ravel(Y[i]):
            test=test+1
            print test

    print test
    test=test/11.0
    print test

#faceCrop('jaffe/*.tiff')
#FaceData('jaffe/*.tiff')
clf = svm.SVC()
FaceTrain()

#FaceTest('KR.SU2.81.tiff')
#FaceTest('KA.AN3.41.tiff')
#FaceTest('MK.FE1.131.tiff')
#FaceTest('she-angry.jpg')
#FaceTest('KA.SA3.35.tiff')
#FaceTest('sad.jpg')

cap = cv2.VideoCapture(1)
cnt=0
str="Start"
while(True):
    # Capture frame-by-frame
    cnt = cnt+1
    ret, frame = cap.read()
    # Our operations on the frame come here
    #gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # Display the resulting frame
    #cv2.putText(gray,"Hello World!!!", (100,100), cv2.FONT_ITALIC, 1, 255)
    frame1=frame
    frame=cv2.resize(frame,(320,249),interpolation=cv2.INTER_CUBIC)
    mat_array = cv.fromarray(frame)
    cv.SaveImage('test.jpg',mat_array)
    if cnt == 5:
        str=FaceTest('test.jpg')
        cnt = 0

    textColor = (0, 0, 255)  # red
    cv2.putText(frame1, str, (50, 50),cv2.FONT_HERSHEY_PLAIN, 3.0, textColor,4,cv2.CV_AA)
    cv2.imshow('frame',frame1)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()








