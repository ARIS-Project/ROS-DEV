# special thanks to http://hanzratech.in/2015/02/24/handwritten-digit-recognition-using-opencv-sklearn-and-python.html
from PIL import Image
import numpy
import cv2
from sklearn.externals import joblib
from skimage.feature import hog
from matplotlib import pyplot as plt
def bw_threshold(T,image):
    h = image.shape[0]
    w = image.shape[1]
    imageCopy = numpy.zeros(shape=(h,w))
    for y in range(0,h):
        for x in range(0,w):
            imageCopy[y,x] = 255 if image[y,x] == T else 0    
    cv8uc = cv2.normalize(imageCopy, None, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8UC1)
    return cv8uc
im_gray = cv2.imread("/home/kapil/Downloads/map_whitebackground.png",cv2.IMREAD_GRAYSCALE)
clf = joblib.load("/home/kapil/Downloads/digitRecognition/digitRecognition/digits_cls.pkl")
im_bwG = bw_threshold(149,im_gray)
im_bwBl = bw_threshold(0,im_gray)

# Find contours in the image
image,ctrs, hier = cv2.findContours(im_bwBl, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

# Get rectangles contains each contour
rects = [cv2.boundingRect(ctr) for ctr in ctrs]

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
    nbr = clf.predict(numpy.array([roi_hog_fd], 'float64'))
    print str(int(nbr[0]))
#im_bwB = bw_threshold(29,im_gray)
#im_bwR = bw_threshold(76,im_gray)
plt.subplot(1,2,1),plt.imshow(im_gray, cmap = 'gray', interpolation= 'bicubic')
plt.subplot(1,2,2),plt.imshow(im_bwBl)
#plt.subplot(2,2,3),plt.imshow(im_bwB)
#plt.subplot(2,2,4),plt.imshow(im_bwR)
plt.xticks([]), plt.yticks([])
plt.show()
