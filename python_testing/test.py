import numpy as np

# Example of a weight matrix
weights = np.random.randn(10, 10)  # Random initialization of weights

# Define the learning rate
learning_rate = 0.01

# Example loss gradient (normally computed during backpropagation)
gradients = np.random.randn(10, 10)  # Random gradients for illustration

# Gradient Descent Step
weights -= learning_rate * gradients

# Projection Step to enforce non-negative weights
weights = np.maximum(weights, 0)

print(weights)  # All weights should now be non-negative
