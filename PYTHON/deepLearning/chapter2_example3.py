import keras
#The gears of neural networks: tensor operations (2.3)
#A Keras layer instance looks like this:
keras.layers.Dense(512, activation = 'relu')

#output = relu(dot(W, input) + b)
#elemnt wise relu
def naive_relu(x):
  assert len(x.shape) == 2
  #x is a 2D Numpy tensor.
  z = x.copy()
  #Avoid overwriting the input tensor.
  for i in range(z.shape[0]):
    for j in range(z.shape[1]):
      z[i, j] = max(z[i, j], 0)
  return z
#element wise addition
def naive_add(x, y):
  assert len(x.shape) == 2
  assert x.shape == y.shape
  z = x.copy()
  for i in range(z.shape[0]):
    for j in range(z.shape[1]):
      z[i, j] += y[i, j]
  return z

#Numpy way (blazing fast)
'''
import numpy as np
z = x + y # Element-wise addition
z = np.maximum(z, 0.) #Element-wise relu
'''

'''
  Broadcasting consists of two steps:
  1)Axes (called broadcast axes) are added to the smaller tensor to match the ndim of
  the larger tensor.
  2)The smaller tensor is repeated alongside these new axes to match the full shape
  of the larger tensor.
'''

def naive_add_matrix_and_vector(x, y):
  assert len(x.shape) == 2
  assert len(y.shape) == 1
  assert x.shape[1] == y.shape[0]
  z = x.copy()
  for i in range(z.shape[0]):
    for j in range(z.shape[1]):
      z[i, j] += y[j]
  return z

#Numpy way
import numpy as np
x = np.random.random((64, 3, 32, 10))
y = np.random.random((32, 10))
z = np.maximum(x, y)

print(x.shape)
print(y.shape)
print(z.shape)

#The dot operation, also called a tensor product (not to be confused with an element-
#wise product)
#import numpy as np
#z = np.dot(x, y)

def naive_vector_dot(x, y):
  assert len(x.shape) == 1 # x,y are numpy vectors
  assert len(y.shape) == 1
  assert x.shape[0] == y.shape[0]
  z = 0.
  for i in range(x.shape[0]):
    z += x[i] * y[i]
  return z

def naive_matrix_vector_dot(x, y):
  assert len(x.shape) == 2 # x= matrix
  assert len(y.shape) == 1 #y = vector
  assert x.shape[1] == y.shape[0] #The first dimension of x must be the same as the 0th dimension of y!
  z = np.zeros(y.shape[0])#This operation returns a vector of 0s with the same shape as y.
  for i in range(x.shape[0]):
    for j in range(x.shape[1]):
      z[i] += x[i, j] * y[j]
  return z
  
def naive_matrix_dot(x, y):
  assert len(x.shape) == 2
  assert len(y.shape) == 2
  assert x.shape[1] == y.shape[0] #The first dimension of x must be the same as the 0th dimension of y!
  z = np.zeros((x.shape[0], y.shape[1])) #This operation returns a matrix of 0s with a specific shape.
  for i in range(x.shape[0]): # iterates over the rows of x
    for j in range(y.shape[1]):# and the collumns of y
      row_x = x[i, :]
      column_y = y[:, j]
      z[i, j] = naive_vector_dot(row_x, column_y)
  return z

#Tensor reshaping

x = np.array([[0, 1], [2, 3], [4, 5]])
print(x.shape)
print(x)
x = x.reshape((6, 1))
print(x.shape)
print(x)
x = x.reshape((2, 3))
print(x.shape)
print(x)
#Transposing a matrix means exchanging its rows and its columns, so that x[i, :] becomes x[:, i]
x = np.random.random_sample((300, 20))
print(x.shape)
print(x)
x = np.transpose(x)
print(x.shape)
print(x)
x = np.transpose(x)#back to normal
print(x.shape)
print(x)


#Geometric interpretation of tensor operations
'''
A = [0.5, 1] # point in 2D space = vector
geometrical ops can be expressed as tensor ops
rotate 2D vector by angle theta can be achieved via dot product with a 2x2 matrix R = [u,v]
where u and v are vectors u = [cos(theta), sin(theta)] and v = [-sin(theta), cos(theta)]

'''