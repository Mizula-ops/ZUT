
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
import csv
from sklearn.naive_bayes import GaussianNB

def dyskretyzacja(data, n):
    row, col = data.shape
    for i in range(col):
        xmin = data[:, i].min()
        xmax = data[:, i].max()
        w = (xmax - xmin) / n
        for j in range(row):
            x = data[j, i]
            data[j, i] = int(min(n - 1, (x - xmin) / w))
    return data.astype(int)


class NBC:
    def __init__(self, classesX, Laplace,Log):
        self.n = classesX
        self.Laplace = Laplace
        self.Log = Log

    def fit(self, X, Y):
        self.X_train = X
        self.Y_train = Y
        rows, cols = X.shape

        classesY, counts = np.unique(Y, return_counts=True)
        self.classesY = classesY
        self.countsY = counts
        self.prioriY = counts / len(Y)

        y_classes = len(classesY)

        sectionN = np.zeros((y_classes, cols, self.n))
        prioriXY = np.zeros((y_classes, cols, self.n))
        for c in range(cols):
            for r in range(rows):
                val = X[r, c]
                indexY = 0
                for j in range(y_classes):
                    if classesY[j] == Y[r]:
                        indexY = j
                        break
                sectionN[indexY, c, val] += 1
            for y in range(y_classes):
                for n in range(self.n):
                    if self.Laplace:
                        prioriXY[y, c, n] = (sectionN[y, c, n] + 1) / (counts[y] + self.n)
                    else:
                        prioriXY[y, c, n] = sectionN[y, c, n] / counts[y]
        self.prioriXY = prioriXY

    def predict(self, X, predict_probality):
        X_test = X
        predictions = np.zeros(len(self.prioriY))
        for i in range(len(self.prioriY)):
            if self.Log==True:
                probability = np.log(self.prioriY[i])
            else:
                probability = self.prioriY[i]

            for x in range(len(X_test)):
                if self.Log == True:
                    probability += np.log(self.prioriXY[i, x, X_test[x]])
                else:
                    probability *= self.prioriXY[i, x, X_test[x]]
            predictions[i] = probability

        if predict_probality == False:
            best_idx = np.argmax(predictions)
            return self.classesY[best_idx]

        SumLikehood = np.sum(predictions)
        if SumLikehood > 0:
            predictions = predictions / SumLikehood
        best_idx = np.argmax(predictions)
        prob_best = predictions[best_idx]
        return prob_best, self.classesY[best_idx]

    def precision(self, X, Y):
        hit = 0
        n_classes = len(self.classesY)
        hitPerClasses = np.zeros(n_classes)
        confusionMatrix = np.zeros((n_classes, n_classes))
        counts_local = np.zeros(n_classes)
        for i in range(X.shape[0]):
            result = self.predict(X[i, :], False)
            indexY = 0
            predIdx = 0

            for j in range(n_classes):
                if self.classesY[j] == Y[i]:
                    indexY = j
                    counts_local[j] += 1
                if self.classesY[j] == result:
                    predIdx = j

            if result == Y[i]:
                hit += 1
                hitPerClasses[indexY] += 1

            confusionMatrix[indexY, predIdx] += 1

        total_accuracy = hit / Y.shape[0]
        accuracy = np.zeros(n_classes)
        for i in range(n_classes):
            if counts_local[i] > 0:
                accuracy[i] = hitPerClasses[i] / counts_local[i]
            else:
                accuracy[i] = 0.0
        confusionMatrixY = confusionMatrix
        return total_accuracy, accuracy, confusionMatrixY


def createdataSets(data, N, Laplace, Log,TestSamples,Gaussian=False):
    df = pd.read_csv(data, sep=r"\s+",header=None)
    df.columns = ["Area", "Perimeter", "Compactness", "KernelLength", "KernelWidth", "Asymmetry", "GrooveLength",
                  "Class"]
    X = df.iloc[:, :-1].to_numpy()
    y = df["Class"].to_numpy()
    if not Gaussian:
        X = dyskretyzacja(X, N)

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=TestSamples, random_state=10)
    if Gaussian:
        Nb = GaussianNB()
        Nb.fit(X_train, y_train)
    else:
        Nb = NBC(N, Laplace,Log)
        Nb.fit(X_train, y_train)
    return Nb, X_train, X_test, y_train, y_test

def createdataSetsIris(data, N, Laplace, Log,TestSamples,Gaussian=False):
    df = pd.read_csv(data)
    X = df.iloc[:, 1:-1].to_numpy()
    mapping = {"Iris-setosa": 1, "Iris-versicolor": 2, "Iris-virginica": 3}
    y = df["Species"].map(mapping).to_numpy()
    if not Gaussian:
        X = dyskretyzacja(X, N)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=TestSamples, random_state=10)
    if Gaussian:
        Nb = GaussianNB()
        Nb.fit(X_train, y_train)
    else:
        Nb = NBC(N, Laplace,Log)
        Nb.fit(X_train, y_train)
    return Nb, X_train, X_test, y_train, y_test

def saveBasic(type,NumberSamples, Predict, Real, Correclty, probability):
    with open('test_basic.csv', type, newline='') as csvfileBasic:
        writerBasic = csv.writer(csvfileBasic)
        writerBasic.writerow([NumberSamples, Predict, Real, Correclty, probability])

def saveExp(type,Id,Name,Model,N,Laplace,Log,TrainSamples,TestSamples,AccForTrain,AccForTest,AccPerClass):
    with open('eksperymenty.csv', type, newline='') as csvfileExp:
        writerExp = csv.writer(csvfileExp)
        writerExp.writerow([Id,Name,Model,N,Laplace,Log,TrainSamples,TestSamples,AccForTrain,AccForTest,AccPerClass])

########################NAGLOWKI##############
saveBasic("w","Number Samples", "Predict", "Real", "Correclty", "probability")
saveExp("w","Id", "Name", "Model", "N", "Laplace", "Log","Train Samples", "Test Samples","AccForTrain", "AccForTest", "AccPerClass")

#############ZAD1##############
n=3
TestSamples=0.2
Nb, X_train, X_test, y_train, y_test=createdataSets("seeds_dataset.txt",n,False,False,TestSamples)
for row in range(len(y_test)):
    probability, predictY = Nb.predict(X_test[row, :], True)
    saveBasic("a",row,predictY,y_test[row],int(predictY == y_test[row]),f"{probability:.2f}")

totalAccTrain, accPerClassTrain, matrixTrain = Nb.precision(X_train, y_train)
totalAccTest, accPerClassTest, matrixTest = Nb.precision(X_test, y_test)
accPerClassTrain=[float(f"{x:.2f}") for x in accPerClassTrain]
accPerClassTest=[float(f"{x:.2f}") for x in accPerClassTest]
saveExp("a","1.Basic","Seeds","Discrete",n,"No","No",1-TestSamples,TestSamples,f"{totalAccTrain:.2f}",f"{totalAccTest:.2f}",accPerClassTest)

#############ZAD2##############
Nb, X_train, X_test, y_train, y_test=createdataSets("seeds_dataset.txt",n,True,True,TestSamples)
totalAccTrain, accPerClassTrain, matrixTrain = Nb.precision(X_train, y_train)
totalAccTest, accPerClassTest, matrixTest = Nb.precision(X_test, y_test)
accPerClassTrain=[float(f"{x:.2f}") for x in accPerClassTrain]
accPerClassTest=[float(f"{x:.2f}") for x in accPerClassTest]
saveExp("a","2.Log","Seeds","Discrete",n,"No","Yes",1-TestSamples,TestSamples,f"{totalAccTrain:.2f}",f"{totalAccTest:.2f}",accPerClassTest)

#############ZAD3##############
Nb, X_train, X_test, y_train, y_test=createdataSets("seeds_dataset.txt",n,True,False,TestSamples)
totalAccTrain, accPerClassTrain, matrixTrain = Nb.precision(X_train, y_train)
totalAccTest, accPerClassTest, matrixTest = Nb.precision(X_test, y_test)
accPerClassTrain=[float(f"{x:.2f}") for x in accPerClassTrain]
accPerClassTest=[float(f"{x:.2f}") for x in accPerClassTest]
saveExp("a","3.Laplace","Seeds","Discrete",n,"Yes","No",1-TestSamples,TestSamples,f"{totalAccTrain:.2f}",f"{totalAccTest:.2f}",accPerClassTest)

#############ZAD4##############
for TestN in range(2,7):
    Nb, X_train, X_test, y_train, y_test = createdataSets("seeds_dataset.txt", TestN, False, False, TestSamples)
    totalAccTrain, accPerClassTrain, matrixTrain = Nb.precision(X_train, y_train)
    totalAccTest, accPerClassTest, matrixTest = Nb.precision(X_test, y_test)
    accPerClassTrain = [float(f"{x:.2f}") for x in accPerClassTrain]
    accPerClassTest = [float(f"{x:.2f}") for x in accPerClassTest]
    saveExp("a", "4.Different N", "Seeds", "Discrete", TestN, "No", "No",1-TestSamples,TestSamples, f"{totalAccTrain:.2f}",f"{totalAccTest:.2f}", accPerClassTest)

#############ZAD5##############
for Samples in np.arange(0.5,0.1,-0.1):
    Nb, X_train, X_test, y_train, y_test = createdataSets("seeds_dataset.txt", n, False, False, Samples)
    totalAccTrain, accPerClassTrain, matrixTrain = Nb.precision(X_train, y_train)
    totalAccTest, accPerClassTest, matrixTest = Nb.precision(X_test, y_test)
    accPerClassTrain = [float(f"{x:.2f}") for x in accPerClassTrain]
    accPerClassTest = [float(f"{x:.2f}") for x in accPerClassTest]
    saveExp("a", "4.Different Samples", "Seeds", "Discrete", n, "No", "No", f"{1-Samples:.1f}",f"{Samples:.1f}", f"{totalAccTrain:.2f}", f"{totalAccTest:.2f}", accPerClassTest)


#############ZAD6##############
Nb, X_train, X_test, y_train, y_test=createdataSets("seeds_dataset.txt",n,True,False,TestSamples,True)

totalAccTrain = Nb.score(X_train, y_train)
totalAccTest = Nb.score(X_test, y_test)

classes=np.unique(y_train[:])
lenClass=len(classes)
sumHit=np.zeros(lenClass)
counts=np.zeros(lenClass)
y_pred=(Nb.predict(X_test))
accPerClassTest = np.zeros(lenClass)
indexY=0
for row in range(len(y_test[:])):
    for j in range(len(classes)):
        if classes[j] == y_test[row]:
            indexY = j
            counts[j] += 1
    if y_pred[row] == y_test[row]:
        sumHit[indexY] += 1
for i in range(lenClass):
    accPerClassTest[i] = sumHit[i] / counts[i]

accPerClassTest=[float(f"{x:.2f}") for x in accPerClassTest]
saveExp("a","6.GaussianNB","Seeds","Continous",n,"No","No",1-TestSamples,TestSamples,f"{totalAccTrain:.2f}",f"{totalAccTest:.2f}",accPerClassTest)
#############ZAD7##############

Nb, X_train, X_test, y_train, y_test=createdataSetsIris("Iris.csv",n,False,False,TestSamples)
totalAccTrain, accPerClassTrain, matrixTrain = Nb.precision(X_train, y_train)
totalAccTest, accPerClassTest, matrixTest = Nb.precision(X_test, y_test)
accPerClassTrain=[float(f"{x:.2f}") for x in accPerClassTrain]
accPerClassTest=[float(f"{x:.2f}") for x in accPerClassTest]
saveExp("a","7.Iris","Iris","Discrete",n,"No","No",1-TestSamples,TestSamples,f"{totalAccTrain:.2f}",f"{totalAccTest:.2f}",accPerClassTest)

