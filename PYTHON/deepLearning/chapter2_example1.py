# chapter 2.1 example
# get the MNIST dataset module
from keras.datasets import mnist
(train_images, train_labels), (test_images, test_labels) = mnist.load_data()
'''
train_images and train_labels form the training set the model will learn from
The model will then be tested on the test set, test_images and test_labels .
The images are encoded as Numpy arrays, and the labels are an array of digits,
ranging from 0 to 9. The images and labels have a one-to-one correspondence.
'''
print("\nExamining training data...")
print("train image shape : %s") % str(train_images.shape)
print("train labels length : %d") % len(train_labels)# = tensor
print("train array looks like : %s\n") % str(train_labels) #numpy.ndarray
print("Examining testing data...")
print("test image shape : %s") % str(test_images.shape)
print("test labels length : %d") % len(test_labels)
print("test array looks like : %s\n") % str(test_labels) #numpy.ndarray
# get keras models and layers modules
from keras import models
from keras import layers
network = models.Sequential() # sequential nn of 2 dense layers
network.add(layers.Dense(512, activation = 'relu', input_shape = (28 * 28,)))
'''2nd layer will return an array of 10 probability scores (summing to 1).
Each score will be the probability that the current digit image belongs to
one of our 10 digit classes.'''
network.add(layers.Dense(10, activation = 'softmax'))
#compile the nn
network.compile(optimizer = 'rmsprop',
                loss = 'categorical_crossentropy',
                metrics = ['accuracy'])
'''
transform arrays of shape (60000, 28, 28) of type
uint8 with values in the [0, 255] interval,
into float32 arrays of shape (60000, 28 * 28) with values between 0 and 1.
'''
train_images = train_images.reshape((60000, 28 * 28))
train_images = train_images.astype('float32') / 255
test_images = test_images.reshape((10000, 28 * 28))
test_images = test_images.astype('float32') / 255
#categorically encode the labels
from keras.utils import to_categorical
train_labels = to_categorical(train_labels)
test_labels = to_categorical(test_labels)
#train the network : fit the model to its training data:
network.fit(train_images, train_labels, epochs = 10, batch_size = 128)
#give the test data now
test_loss, test_acc = network.evaluate(test_images, test_labels)
print("test accuracy = %f") % test_acc
print("test loss = %f") % test_loss
'''
This gap between training accuracy and test accuracy is an example of
overfitting: the fact that machine-learning models 
tend to perform worse on new data than on their training data
'''