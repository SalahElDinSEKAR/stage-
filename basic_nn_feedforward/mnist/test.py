import numpy as np

def read_idx(filename):
    with open(filename, 'rb') as f:
        # Read magic number
        magic = int.from_bytes(f.read(4), byteorder='big')
        if magic == 2051:
            # Image file
            num_images = int.from_bytes(f.read(4), byteorder='big')
            num_rows = int.from_bytes(f.read(4), byteorder='big')
            num_cols = int.from_bytes(f.read(4), byteorder='big')
            images = np.frombuffer(f.read(), dtype=np.uint8).reshape(num_images, num_rows, num_cols)
            return images
        elif magic == 2049:
            # Label file
            num_labels = int.from_bytes(f.read(4), byteorder='big')
            labels = np.frombuffer(f.read(), dtype=np.uint8)
            return labels
        else:
            raise ValueError("Invalid magic number")

# File paths
train_images_file = '/home/sekars/Documents/stage/basic_nn_feedforward/Neural-Network-MNIST-CPP-master/mnist/train-images.idx3-ubyte'
train_labels_file = '/home/sekars/Documents/stage/basic_nn_feedforward/Neural-Network-MNIST-CPP-master/mnist/train-labels.idx1-ubyte'
test_images_file = '/home/sekars/Documents/stage/basic_nn_feedforward/Neural-Network-MNIST-CPP-master/mnist/t10k-images.idx3-ubyte'
test_labels_file = '/home/sekars/Documents/stage/basic_nn_feedforward/Neural-Network-MNIST-CPP-master/mnist/t10k-labels.idx1-ubyte'

# Read and inspect training images
train_images = read_idx(train_images_file)
print("Training images shape:", train_images.shape)

# Read and inspect training labels
train_labels = read_idx(train_labels_file)
print("Training labels shape:", train_labels.shape)

# Read and inspect test images
test_images = read_idx(test_images_file)
print("Test images shape:", test_images.shape)

# Read and inspect test labels
test_labels = read_idx(test_labels_file)
print("Test labels shape:", test_labels.shape)