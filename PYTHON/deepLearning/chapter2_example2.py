#datasets, shapes representation and batches (2.2)
from keras.datasets import mnist
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
#3d tensor of 8-bit integers, 60000 images of 28 x 28 size
print(train_images.ndim)
print(train_images.shape)
print(train_images.dtype)

#display the fourth digit in this 3D tensor
digit = train_images[4]
import matplotlib.pyplot as plt
#plt.imshow(digit, cmap = plt.cm.binary)
#plt.show()

#Manipulating tensors in Numpy
#get 90 images of same size
my_slice1 = train_images[10:100, 0:28, 0:28]
#my_slice = train_images[10:100, :, :] == equivalent
print(my_slice1.shape)
cutDigit = my_slice1[4]
plt.imshow(cutDigit, cmap = plt.cm.binary)
plt.show()
#get the bottom right part
my_slice2 = train_images[10:100, 0:14, 0:14]
print(my_slice2.shape)
cutDigit = my_slice2[4]
plt.imshow(cutDigit, cmap = plt.cm.binary)
plt.show()
#get the centermost part (negative indexes)
my_slice3 = train_images[10:100, 7:-7, 7:-7]
print(my_slice3.shape)
cutDigit = my_slice3[4]
plt.imshow(cutDigit, cmap = plt.cm.binary)
plt.show()

#deep learning models dont process an entire dataset at once
#rather, they break the data into small batches

First_batch = train_images[:128]
Second_batch = train_images[128:256]
'''
  Nth_batch = train_images[128 * n:128 * (n + 1)]
  When considering such a batch tensor, the first axis (axis 0) is called the batch axis or
  batch dimension
'''

'''
vector_data = 2DTensor.shape = (10000, 3)
10000 people as the base axis = sample(0)axis
3 features as the features(1)axis = age, zip code, income

A dataset of stock prices. Every minute, we store the current price of the stock,
the highest price in the past minute, and the lowest price in the past minute.
Thus every minute is encoded as a 3D vector, an entire day of trading is
encoded as a 2D tensor of shape (1440, 3) (there are 1440 minutes in a trading
day), and 365 days’ worth of data can be stored in a 3D tensor of shape (365,
1440, 3) . Here, each sample would be one day’s worth of data.

Images typically have three dimensions: height, width, and color depth. Although
grayscale images (like our MNIST digits) have only a single color channel and could
thus be stored in 2D tensors, by convention image tensors are always 3D , with a one-
dimensional color channel for grayscale images. A batch of 128 grayscale images of
size 256 × 256 could thus be stored in a tensor of shape (128, 256, 256, 1) , and a
batch of 128 color images could be stored in a tensor of shape (128, 256, 256, 3)

For instance, a 60-second, 144 × 256 YouTube video clip sampled at 4 frames per
second would have 240 frames. A batch of four such video clips would be stored in a
tensor of shape (4, 240, 144, 256, 3) . That’s a total of 106,168,320 values! If the
dtype of the tensor was float32 , then each value would be stored in 32 bits, so the
tensor would represent 405 MB. Heavy! Videos you encounter in real life are much
lighter, because they aren’t stored in float32 , and they’re typically compressed by a
large factor (such as in the MPEG format).

'''

