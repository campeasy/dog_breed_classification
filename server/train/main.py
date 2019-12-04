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


def train_classifier(model, train_loader, val_loader, exp_name='main', lr=0.001, epochs=10, momentum=0.9):
    criterion = CrossEntropyLoss()
    optimizer = SGD(model.parameters(), lr, momentum=momentum)
    #meters
    loss_meter = AverageValueMeter()
    acc_meter = AverageValueMeter()
    #device
    device = "cuda" if torch.cuda.is_available() else "cpu"
    model.to(device)
    #definiamo un dizionario contenente i loader di training e test
    loader = {
        'Train' : train_loader,
        'Val' : val_loader
    }
    ep = EpochsPlotter(folder_path=os.path.join("plots", exp_name), labels=['Train', 'Val'], columns=['Loss', 'Accuracy'])
    for e in range(epochs):
        #iteriamo tra due modalità: train e test
        for mode in ['Train', 'Val']:
            loss_meter.reset()
            acc_meter.reset()
            if mode == 'Train':
                model.train()
            else:
                model.eval()
            with torch.set_grad_enabled(mode=='Train'): #abilitiamo i gradienti solo in training
                t = tqdm(loader[mode])
                for i, batch in enumerate(t):
                    x = batch[0].to(device) #"portiamoli sul device corretto"
                    y = batch[1].to(device)
                    output = model(x)
                    l = criterion(output,y)
                    
                    if mode=='Train':
                        l.backward()
                        optimizer.step()
                        optimizer.zero_grad()
                        
                    acc = accuracy_score(y.to('cpu'),output.to('cpu').max(1)[1])
                    n = batch[0].shape[0] #numero di elementi nel batch
                    loss_meter.add(l.item()*n,n)
                    acc_meter.add(acc*n,n)
                
            ep.set(mode, e, {'Loss' : loss_meter.value()[0], 'Accuracy' : acc_meter.value()[0]})
            ep.save()
            ep.plot()

        params_path = os.path.join("parameters", exp_name, "{}-{}.pth".format(exp_name, e))
        os.makedirs(params_path, exist_ok=True)
        torch.save(model.state_dict(), '%s-%d.pth'%(exp_name,e))
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

    image_net_train = ImageNet(root='F:\PytorchMatchingNetwork\imagenet', split="train", transform=train_transform)
    image_net_val = ImageNet(root='F:\PytorchMatchingNetwork\imagenet', split="val", transform=val_transform)
    image_net_train_loader = DataLoader(image_net_train, batch_size=24, num_workers=4, shuffle=True)
    image_net_val_loader = DataLoader(image_net_val, batch_size=24, num_workers=4)

    model = resnet50(pretrained=False)
    train_classifier(model, image_net_train_loader, image_net_val_loader)

if __name__ == "__main__":
    main()
