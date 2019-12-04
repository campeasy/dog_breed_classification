import os
import numpy as np
from tqdm import tqdm
import torch
import torch.nn
from torch.nn import Module, Linear, CrossEntropyLoss
from torch.optim import SGD
from torchvision import transforms
from torchvision.models import resnet50
from torchnet.meter import AverageValueMeter
from torchvision.datasets import ImageNet
from torch.utils.data import DataLoader
from sklearn.metrics import accuracy_score
from epochsplotter import EpochsPlotter
from dogimagenet import DogImageNet


def train_classifier(model, train_loader, val_loader, exp_name='experiment', starting_epoch=0, lr=0.005, epochs=100, momentum=0.9):
    device = "cuda" if torch.cuda.is_available() else "cpu"

    criterion = CrossEntropyLoss().to(device)
    optimizer = SGD(model.parameters(), lr, momentum=momentum)

    loss_meter = AverageValueMeter()
    acc_meter = AverageValueMeter()

    loader = {
        'Train' : train_loader,
        'Val' : val_loader
    }

    ep_folder_path = os.path.join("plots", exp_name)
    if starting_epoch == 0:
        ep = EpochsPlotter(folder_path=ep_folder_path, labels=['Train', 'Val'], columns=['Loss', 'Accuracy'])
    else:
        ep = EpochsPlotter(folder_path=ep_folder_path, load=True)
        params_path = os.path.join("parameters", exp_name, "epoch-{}.pth".format(starting_epoch-1))
        print("Loading {}".format(params_path))
        model.load_state_dict(torch.load(params_path))

    model.to(device)
    for e in range(starting_epoch, starting_epoch + epochs):
        #iteriamo tra due modalità: train e test
        for mode in ['Train', 'Val']:
            loss_meter.reset()
            acc_meter.reset()
            if mode == 'Train':
                model.train()
            else:
                model.eval()
            with torch.set_grad_enabled(mode=='Train'):
                t = tqdm(loader[mode])
                for i, batch in enumerate(t):
                    x = batch[0].to(device)
                    y = batch[1].to(device)
                    output = model(x)
                    l = criterion(output, y)
                    
                    if mode=='Train':
                        l.backward()
                        optimizer.step()
                        optimizer.zero_grad()
                        
                    acc = accuracy_score(y.to('cpu'), output.to('cpu').max(1)[1])
                    batch_size = batch[0].shape[0]
                    loss_meter.add(l.item()*batch_size, batch_size)
                    acc_meter.add(acc*batch_size, batch_size)

                    desc = "loss = {:06.4f}, accuracy = {:05.3f} ".format(loss_meter.value()[0], acc_meter.value()[0])
                    t.set_description(desc)
                t.refresh()
                t.close()
                
            ep.set(mode, e, {'Loss' : loss_meter.value()[0], 'Accuracy' : acc_meter.value()[0]})
            ep.save()
            ep.plot()

        params_path = os.path.join("parameters", exp_name)
        os.makedirs(params_path, exist_ok=True)
        params_path = os.path.join(params_path, "epoch-{}.pth".format(e))
        torch.save(model.state_dict(), params_path)
    return model

def main():
    train_transform = transforms.Compose([
        transforms.RandomResizedCrop(224),
        transforms.RandomHorizontalFlip(),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ])

    val_transform = transforms.Compose([
        transforms.Resize(256),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ])

    image_net_train = DogImageNet(root='F:\PytorchMatchingNetwork\imagenet', split="train", transform=train_transform)
    image_net_val = DogImageNet(root='F:\PytorchMatchingNetwork\imagenet', split="val", transform=val_transform)
    image_net_train_loader = DataLoader(image_net_train, batch_size=24, num_workers=4, shuffle=True)
    image_net_val_loader = DataLoader(image_net_val, batch_size=24, num_workers=4)

    model = resnet50(pretrained=False)
    model.fc = Linear(in_features=2048, out_features=len(image_net_train.classes), bias=True)
    train_classifier(model, image_net_train_loader, image_net_val_loader, exp_name="maindog", starting_epoch = 46, lr=0.0005)

if __name__ == "__main__":
    main()
