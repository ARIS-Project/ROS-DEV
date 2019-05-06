# special thanks to http://hanzratech.in/2015/02/24/handwritten-digit-recognition-using-opencv-sklearn-and-python.html
import numpy
import cv2
from sklearn.externals import joblib
from skimage.feature import hog
from matplotlib import pyplot as plt
import math
import os.path
def bw_threshold(T,image):
    h = image.shape[0]
    w = image.shape[1]
    imageCopy = numpy.zeros(shape=(h,w))
    for y in range(0,h):
        for x in range(0,w):
            imageCopy[y,x] = 255 if (image[y,x] == T) else 0
    # convert the ndarray to CV8UC1 format otherwise this array can't find contours    
    cv8uc = cv2.normalize(imageCopy, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8UC1)
    return cv8uc

def min_dist(arr,final,key,numCenter):
    gRows = centroidsG.shape[0]
    x1 = numCenter[0]
    y1 = numCenter[1]
    cnt = 0
    smallest = 0
    #print str(x1) + "," + str(y1)
    for gObjCnter in range(1,gRows,1):
        x = centroidsG[gObjCnter][0]
        y = centroidsG[gObjCnter][1]
        smallest1 = math.sqrt(pow((x1 - x),2) + pow((y1-y),2))
        if(cnt == 0):
            smallest = smallest1
            final[dictKey] = numpy.array((x,y))
            #print str(dictKey) + "\n" + str(x) + "," + str(y)
        else:
            if (smallest1 < smallest):
                final[dictKey] = numpy.array((x,y))
                smallest = smallest1
                #print str(dictKey) + "\n" + str(x) + "," + str(y) 
        cnt = cnt + 1

def assign_zero(final,x,y):
    for dictValue in final.values():
        if (x == dictValue[0] and y == dictValue[1]):
            return True
    return False

prompt = '> '
print "Give the path to your image:" 
image_file_name = raw_input(prompt)
# Take out gray scale images from RGB images    
#im_gray = cv2.imread("/home/kapil/Downloads/map_whitebackground.png",cv2.IMREAD_GRAYSCALE)
im_gray = cv2.imread(image_file_name,cv2.IMREAD_GRAYSCALE)

prompt = '> '
print "Give the path to your Machine Learning Model:" 
Model_file_name = raw_input(prompt)
# To load the prediction classifier
#clf = joblib.load("/home/kapil/Downloads/digitRecognition/digitRecognition/digits_cls.pkl")
clf = joblib.load(Model_file_name)
#find binary image for green images
im_bwG = bw_threshold(149,im_gray)

# Find centroids in the image with just green objects
nlabels,labels,stats,centroidsG = cv2.connectedComponentsWithStats(im_bwG)

#find binary image for black objects
im_bwBl = bw_threshold(0,im_gray)

# Find contours in the image with just numbers
image,ctrs, hier = cv2.findContours(im_bwBl, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

# Get rectangles contains each contour
rects = [cv2.boundingRect(ctr) for ctr in ctrs]

#Python dictionry to store coordinates of centre of numbers against digits
numCenterDict = {}

for rect in rects:
    # find centres of bounding rectangular boxes
    x = (((2*rect[0])+rect[2])/2)
    y = (((2*rect[1])+rect[3])/2)
    print str(x) + "," + str(y)
    # Make the rectangular region around the digit
    leng = int(rect[3] * 1.6)
    pt1 = int(rect[1] + rect[3] // 2 - leng // 2)
    pt2 = int(rect[0] + rect[2] // 2 - leng // 2)
    roi = im_bwBl[pt1:pt1+leng, pt2:pt2+leng]
    # Resize the image
    roi = cv2.resize(roi, (28, 28), interpolation=cv2.INTER_AREA)
    roi = cv2.dilate(roi, (3, 3))
    # Calculate the HOG features
    roi_hog_fd = hog(roi, orientations=9, pixels_per_cell=(14, 14), cells_per_block=(1, 1), visualise=False)
    # Actual Prediction of numbers
    nbr = clf.predict(numpy.array([roi_hog_fd], 'float64'))
    print str(int(nbr[0]))
    numCenterDict[int(nbr[0])] = numpy.array((x,y))
#im_bwB = bw_threshold(29,im_gray)
#im_bwR = bw_threshold(76,im_gray)

# print the centres alloted to these numbers 
print numCenterDict

#print centroidsG[1][0]

#Loop to find minimum distance between digit centre and door centre and assign centre to the specific number
finalDict = {}
for dictKey in numCenterDict.keys():
    numCenterVal = numCenterDict[dictKey]
    # debug 
    # print str(x1) + "," + str(y1)
    min_dist(centroidsG,finalDict,dictKey,numCenterVal)

# To assign the start position in the dictionary
gRows = centroidsG.shape[0]
for gObjCnter in range(1,gRows,1):
    x = centroidsG[gObjCnter][0]
    y = centroidsG[gObjCnter][1]
    if(not assign_zero(finalDict,x,y)):
        finalDict[0] = numpy.array((x,y))
print finalDict

#To find max x distance equivalent to image
prompt = '> '
print "Give the max x distance equivalent to image:" 
xMax = raw_input(prompt)

#To find max y distance equivalent to image
prompt = '> '
print "Give the max y distance equivalent to image:" 
yMax = raw_input(prompt)

yPixelMax = im_gray.shape[0]
xPixelMax = im_gray.shape[1]
#print "*************max Pixels Available*********"
#print xPixelMax
#print yPixelMAx
#print "*************END**************************"
# find factor for multiplication
dx = float(xMax) / xPixelMax
dy = float(yMax) / yPixelMax
#print dx
#print dy

# Adding the factor from the map
factoredDict = {}
for dictkey in finalDict.keys():
    dictVal = finalDict[dictkey]
    x = dictVal[0] * dx
    y = dictVal[1] * dy
    #print str(x) + "," + str(y)
    #print str(dictkey)
    factoredDict[dictkey] = numpy.array((x,y))

# for posting data to yaml file
#print factoredDict
centerName = "doorData"
print "Please input the location where you want to save the yaml file"
yaml_location = raw_input(prompt)
#centerLocation = "/home/kapil/catkin_ws/src/aris_python/scripts"
centerLocation = yaml_location
completeFileNameYAML = os.path.join(centerLocation, centerName + ".yaml")
yaml = open(completeFileNameYAML, "w")
yaml.write(str(factoredDict))

centerLocation = yaml_location
completeFileNameYAML = os.path.join(centerLocation, centerName + ".txt")
text = open(completeFileNameYAML, "w")
for dictkey in factoredDict.keys():
    dictVal = factoredDict[dictkey]
    x = dictVal[0]
    y = dictVal[1]
    text.write(str(dictkey) + ";" + str(x) + ";" + str(y) + "\n")

# ********************** All for Debugging ****************************
#plt.subplot(1,2,1),plt.imshow(im_gray, cmap = 'gray', interpolation= 'bicubic')
#plt.subplot(1,2,2),plt.imshow(im_bwBl)
#plt.subplot(2,2,3),plt.imshow(im_bwB)
#plt.subplot(2,2,4),plt.imshow(im_bwR)
#plt.xticks([]), plt.yticks([])
#plt.show()
