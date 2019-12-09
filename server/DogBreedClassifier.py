import argparse
import torch
import torchvision.models as models
import torchvision.transforms as transforms

from PIL import Image
from torch.nn import Linear

class DogBreedClassifier():

    def __init__(self, labels_path, weights_path):
        self.dog_labels_start_index = 151
        with open(labels_path) as f:
            self.labels = eval(f.read())
        self.transform = transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
        ])
        self.model = models.resnet50(pretrained=False)
        self.model.fc = Linear(in_features=2048, out_features=118, bias=True)
        self.model.load_state_dict(torch.load(weights_path, map_location=torch.device('cpu')))
        self.model.eval()

    def classify(self, img):
        with torch.no_grad():
            out = self.model(self.transform(img).unsqueeze(0))
            out = out[0].softmax(dim=0)
            out_sort = out.sort(descending=True)
            result = list(zip(out_sort[1][:10].tolist(), out_sort[0][:10].tolist()))
            final_string = ""
            for idx, probability in result:
                current = "{:.2f}%  {}".format(probability*100, self.labels[self.dog_labels_start_index + idx])
                final_string += current + "\n"

            print("[OK - DogBreedClassifier] Image correctly classified")
            return final_string

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='PyTorch Dog Classification Test')
    parser.add_argument('-i', '--image', type=str, help='image path to classify')
    parser.add_argument('-l', '--labels', default="labels.txt", type=str, help='labels path')
    args = parser.parse_args()
    img = Image.open(args.image)
    dbc = DogBreedClassifier(labels_path=args.labels)
    print(dbc.classify(img))
