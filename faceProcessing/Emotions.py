import multiprocessing
import subprocess

def worker(file):
    subprocess.Popen(['screen', 'python'+file])

if __name__ == "__main__":
    files = ["/home/pujandave/Documents/FacialEmotion/linux64_c++_v1.2/videoFeed.py","/home/pujandave/Documents/FacialEmotion/linux64_c++_v1.2/emotionDetect.py"]
    for i in files:
        p = multiprocessing.Process(target=worker(i))
        p.start()
