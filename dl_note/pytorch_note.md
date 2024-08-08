# Pytorch Note



## Pytorch Tutorials

### Pytorch Basics

#### quick start

- 加载数据集
- 模型
- 训练与测试
- 保存与加载模型
- 预测结果

##### import

```python
import torch
from torch import nn
from torch.utils.data import DataLoader
from torchvision import datasets
from torchvision.transforms import ToTensor
```



##### torchvision.datasets

```python
# Download training data from open datasets.
training_data = datasets.FashionMNIST(
    root="data",
    train=True,
    download=True,
    transform=ToTensor(),
)

# Download test data from open datasets.
test_data = datasets.FashionMNIST(
    root="data",
    train=False,
    download=True,
    transform=ToTensor(),
)
```



##### torch.utils.data.Dataloader

```python
batch_size = 64

# Create data loaders.
train_dataloader = DataLoader(training_data, batch_size=batch_size)
test_dataloader = DataLoader(test_data, batch_size=batch_size)

for X, y in test_dataloader:
    print(f"Shape of X [N, C, H, W]: {X.shape}")
    print(f"Shape of y: {y.shape} {y.dtype}")
    break
```



##### device

```python
# Get cpu, gpu or mps device for training.
device = (
    "cuda"
    if torch.cuda.is_available()
    else "mps"
    if torch.backends.mps.is_available()
    else "cpu"
)
print(f"Using {device} device")
```



##### a simple model

```python
# Define model
class NeuralNetwork(nn.Module):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.linear_relu_stack = nn.Sequential(
            nn.Linear(28*28, 512),
            nn.ReLU(),
            nn.Linear(512, 512),
            nn.ReLU(),
            nn.Linear(512, 10)
        )

    def forward(self, x):
        x = self.flatten(x)
        logits = self.linear_relu_stack(x)
        return logits

model = NeuralNetwork().to(device)
print(model)
```



##### loss and optimizer

```
loss_fn = nn.CrossEntropyLoss()
optimizer = torch.optim.SGD(model.parameters(), lr=1e-3)
```



##### train

```python
def train(dataloader, model, loss_fn, optimizer):
    size = len(dataloader.dataset)
    model.train()
    for batch, (X, y) in enumerate(dataloader):
        X, y = X.to(device), y.to(device)

        # Compute prediction error
        pred = model(X)
        loss = loss_fn(pred, y)

        # Backpropagation
        loss.backward()
        optimizer.step()
        optimizer.zero_grad()

        if batch % 100 == 0:
            loss, current = loss.item(), (batch + 1) * len(X)
            print(f"loss: {loss:>7f}  [{current:>5d}/{size:>5d}]")
```



##### test

```python
def test(dataloader, model, loss_fn):
    size = len(dataloader.dataset)
    num_batches = len(dataloader)
    model.eval()
    test_loss, correct = 0, 0
    with torch.no_grad():
        for X, y in dataloader:
            X, y = X.to(device), y.to(device)
            pred = model(X)
            test_loss += loss_fn(pred, y).item()
            correct += (pred.argmax(1) == y).type(torch.float).sum().item()
    test_loss /= num_batches
    correct /= size
    print(f"Test Error: \n Accuracy: {(100*correct):>0.1f}%, Avg loss: {test_loss:>8f} \n")
```



##### run

```python
epochs = 5
for t in range(epochs):
    print(f"Epoch {t+1}\n-------------------------------")
    train(train_dataloader, model, loss_fn, optimizer)
    test(test_dataloader, model, loss_fn)
print("Done!")
```



##### save model

```python
torch.save(model.state_dict(), "model.pth")
print("Saved PyTorch Model State to model.pth")
```



##### load model

```python
model = NeuralNetwork().to(device)
model.load_state_dict(torch.load("model.pth"))
```



##### predict

```python
classes = [
    "T-shirt/top",
    "Trouser",
    "Pullover",
    "Dress",
    "Coat",
    "Sandal",
    "Shirt",
    "Sneaker",
    "Bag",
    "Ankle boot",
]

model.eval()
x, y = test_data[0][0], test_data[0][1]
with torch.no_grad():
    x = x.to(device)
    pred = model(x)
    predicted, actual = classes[pred[0].argmax(0)], classes[y]
    print(f'Predicted: "{predicted}", Actual: "{actual}"')
```



#### Tensors

- Tensors are similar to [NumPy's](https://numpy.org/) ndarrays, except
  that tensors can run on GPUs or other hardware accelerators.
- Tensors are also
  optimized for automatic differentiation

- Import

  ```python
  import torch
  import numpy as np
  ```



##### Init a Tensor

```python
# create from data
data = [[1, 2],[3, 4]]
x_data = torch.tensor(data)

# create from numpy array
np_array = np.array(data)
x_np = torch.from_numpy(np_array)

# create from another tensor with properties
x_ones = torch.ones_like(x_data) # retains the properties of x_data
print(f"Ones Tensor: \n {x_ones} \n")
x_rand = torch.rand_like(x_data, dtype=torch.float) # overrides the datatype of x_data
print(f"Random Tensor: \n {x_rand} \n")

# create from shape
shape = (2,3,)
rand_tensor = torch.rand(shape)
ones_tensor = torch.ones(shape)
zeros_tensor = torch.zeros(shape)
print(f"Random Tensor: \n {rand_tensor} \n")
print(f"Ones Tensor: \n {ones_tensor} \n")
print(f"Zeros Tensor: \n {zeros_tensor}")

# attributes
tensor = torch.rand(3,4)
print(f"Shape of tensor: {tensor.shape}")
print(f"Datatype of tensor: {tensor.dtype}")
print(f"Device tensor is stored on: {tensor.device}")
```



##### Operation on Tensors

- Over 100 tensor operations, including arithmetic, linear algebra, matrix manipulation (transposing, indexing, slicing), sampling and more are comprehensively described [here](https://pytorch.org/docs/stable/torch.html).

```python
# move to GPU
if torch.cuda.is_available():
    tensor = tensor.to("cuda")
    
# Standard numpy-like indexing and slicing
tensor = torch.ones(4, 4)
print(f"First row: {tensor[0]}")
print(f"First column: {tensor[:, 0]}")
print(f"Last column: {tensor[..., -1]}")
tensor[:,1] = 0
print(tensor)
"""
First row: tensor([1., 1., 1., 1.])
First column: tensor([1., 1., 1., 1.])
Last column: tensor([1., 1., 1., 1.])
tensor([[1., 0., 1., 1.],
        [1., 0., 1., 1.],
        [1., 0., 1., 1.],
        [1., 0., 1., 1.]])
"""

# join tensors 拼接 concatenate .cat
t1 = torch.cat([tensor, tensor, tensor], dim=1)
print(t1)
"""
tensor([[1., 0., 1., 1., 1., 0., 1., 1., 1., 0., 1., 1.],
        [1., 0., 1., 1., 1., 0., 1., 1., 1., 0., 1., 1.],
        [1., 0., 1., 1., 1., 0., 1., 1., 1., 0., 1., 1.],
        [1., 0., 1., 1., 1., 0., 1., 1., 1., 0., 1., 1.]])
"""

# Arithmetic operations 算术运算
# 矩阵乘法，这三种方式等效
y1 = tensor @ tensor.T
y2 = tensor.matmul(tensor.T)
y3 = torch.rand_like(y1)
torch.matmul(tensor, tensor.T, out=y3)

# 矩阵内元素乘法
z1 = tensor * tensor
z2 = tensor.mul(tensor)
z3 = torch.rand_like(tensor)
torch.mul(tensor, tensor, out=z3)

# 单元素tensor
agg = tensor.sum()
agg_item = agg.item()
print(agg_item, type(agg_item))
"""
12.0 <class 'float'>
"""

# 就地操作 将结果存储到操作数中的操作称为就地操作。
# 它们用 _ 后缀表示。例如：x.copy_(y), x.t_()，将改变 x.copy_(y)。
# 尽量不要使用就地操作，会失去历史记录影响反向传播
print(f"{tensor} \n")
tensor.add_(5)
print(tensor)
"""
tensor([[1., 0., 1., 1.],
        [1., 0., 1., 1.],
        [1., 0., 1., 1.],
        [1., 0., 1., 1.]]) 

tensor([[6., 5., 6., 6.],
        [6., 5., 6., 6.],
        [6., 5., 6., 6.],
        [6., 5., 6., 6.]])
"""

# Tensor to numpy
# A change in the tensor reflects in the NumPy array.
t = torch.ones(5)
n = t.numpy()
t.add_(1)
print(f"t: {t}")
print(f"n: {n}")
"""
t: tensor([2., 2., 2., 2., 2.])
n: [2. 2. 2. 2. 2.]
"""

# numpy to tensor
# Changes in the NumPy array reflects in the tensor.
n = np.ones(5)
t = torch.from_numpy(n)
np.add(n, 1, out=n)
print(f"t: {t}")
print(f"n: {n}")
"""
t: tensor([2., 2., 2., 2., 2.], dtype=torch.float64)
n: [2. 2. 2. 2. 2.]
"""
```



##### Squeeze Tensor

```python
import torch

# 创建一个张量，其形状为 (1, 3, 1, 5)
# 使用 torch.squeeze 移除所有尺寸为1的维度
x = torch.randn(1, 3, 1, 5)
print("原始张量形状:", x.shape)
y = x.squeeze()
print("移除后张量形状:", y.shape)
"""
原始张量形状: torch.Size([1, 3, 1, 5])
移除后张量形状: torch.Size([3, 5])
"""



# 创建一个张量，其形状为 (1, 3, 1, 5)
# 指定维度移除，尝试移除第0维
x = torch.randn(1, 3, 1, 5)
print("原始张量形状:", x.shape)
y = x.squeeze(0)
print("移除第0维后的张量形状:", y.shape)
# 尝试移除第2维
z = x.squeeze(2)
print("移除第2维后的张量形状:", z.shape)
"""
原始张量形状: torch.Size([1, 3, 1, 5])
移除第0维后的张量形状: torch.Size([3, 1, 5])
移除第2维后的张量形状: torch.Size([1, 3, 5])
"""
```



#### Datasets and Dataloader

##### Load a dataset

```python
import torch
from torch.utils.data import Dataset
from torchvision import datasets
from torchvision.transforms import ToTensor
import matplotlib.pyplot as plt


training_data = datasets.FashionMNIST(
    root="data",
    train=True,
    download=True,
    transform=ToTensor()
)

test_data = datasets.FashionMNIST(
    root="data",
    train=False,
    download=True,
    transform=ToTensor()
)
```



##### Visualize the dataset

```python
labels_map = {
    0: "T-Shirt",
    1: "Trouser",
    2: "Pullover",
    3: "Dress",
    4: "Coat",
    5: "Sandal",
    6: "Shirt",
    7: "Sneaker",
    8: "Bag",
    9: "Ankle Boot",
}
figure = plt.figure(figsize=(8, 8))
cols, rows = 3, 3
for i in range(1, cols * rows + 1):
    sample_idx = torch.randint(len(training_data), size=(1,)).item()
    img, label = training_data[sample_idx]
    figure.add_subplot(rows, cols, i)
    plt.title(labels_map[label])
    plt.axis("off")
    plt.imshow(img.squeeze(), cmap="gray")
plt.show()
```



##### Customize a Dataset

```python
import os
import pandas as pd
from torchvision.io import read_image

class CustomImageDataset(Dataset):
    def __init__(self, annotations_file, img_dir, transform=None, target_transform=None):
        """
        The __init__ function is run once when instantiating the Dataset object.
        We initialize the directory containing the images, the annotations file, 
        and both transforms (covered in more detail in the next section).
        """
        self.img_labels = pd.read_csv(annotations_file)
        self.img_dir = img_dir
        self.transform = transform
        self.target_transform = target_transform

    def __len__(self):
        """
        The __len function returns the number of samples in our dataset.
        """
        return len(self.img_labels)

    def __getitem__(self, idx):
        """
        The __getitem function loads and returns a sample from the dataset at the given index idx. 
        Based on the index, it identifies the image's location on disk, converts that to a tensor using 
        read_image, retrieves the corresponding label from the csv data in self.img_labels, calls the 
        transform functions on them (if applicable), and returns the tensor image and corresponding 
        label in a tuple
        """
        img_path = os.path.join(self.img_dir, self.img_labels.iloc[idx, 0])
        image = read_image(img_path)
        label = self.img_labels.iloc[idx, 1]
        if self.transform:
            image = self.transform(image)
        if self.target_transform:
            label = self.target_transform(label)
        return image, label
```



##### Dataloader

```python
from torch.utils.data import DataLoader

# load data in batches
train_dataloader = DataLoader(training_data, batch_size=64, shuffle=True)
test_dataloader = DataLoader(test_data, batch_size=64, shuffle=True)

# Display image and label.
train_features, train_labels = next(iter(train_dataloader))
print(f"Feature batch shape: {train_features.size()}")
print(f"Labels batch shape: {train_labels.size()}")
img = train_features[0].squeeze()
label = train_labels[0]
plt.imshow(img, cmap="gray")
plt.show()
print(f"Label: {label}")
```



#### Transforms for datasets

```python
import torch
from torchvision import datasets
from torchvision.transforms import ToTensor, Lambda

ds = datasets.FashionMNIST(
    root="data",
    train=True,
    download=True,
	"""
	ToTensor converts a PIL image or NumPy ndarray into a FloatTensor. 
    and scales the image's pixel intensity values in the range [0., 1.]
	"""
    transform=ToTensor(),
    """
    Lambda transforms apply any user-defined lambda function.
    Here, we define a function to turn the integer into a one-hot encoded tensor. 
    It first creates a zero tensor of size 10 (the number of labels in our dataset) 
    and calls scatter_ which assigns a value=1 on the index as given by the label y.
    """
    target_transform=Lambda(lambda y: torch.zeros(10, dtype=torch.float).scatter_(0, torch.tensor(y), value=1))
)
```

