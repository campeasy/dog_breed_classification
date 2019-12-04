import os
import torch
from torch.utils.data import Dataset
from torchvision.datasets import ImageNet
from torchvision import transforms
import pickle

class DogImageNet(Dataset):
    def __init__(self, root="imagenet", split="train", transform=None, download=False):
        self.image_net = ImageNet(root=root, split=split, transform=transform, download=download)
        self.split = split
        self.classes = list(range(151, 269))

        filename = 'index_map_train' if split=="train" else 'index_map_val'
        with open (os.path.join(root, filename), 'rb') as fp:
            self.index_map = pickle.load(fp)

        self.base_index = self.index_map[151][0]
        self.len = 0
        for c in self.classes:
            self.len += self.index_map[c][1]

    def __len__(self):
        return self.len

    def __getitem__(self, target_idx):
        target_idx += self.base_index
        target = self.image_net[target_idx]
        return (target[0], target[1]-151)