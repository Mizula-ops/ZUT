
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import seaborn as sb
import pandas as pd
from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn.model_selection import KFold
from sklearn.neighbors import KDTree
from sklearn.decomposition import PCA
import time

# IMPLEMENTACJA DLA KLASYFIKACJI
class knn_classification:
    def __init__(self,n_neighbours=1,use_KDTree=False):
        self.n_neighbours=n_neighbours
        self.use_KDTree = use_KDTree 
    def fit(self,X,y):
        self.X_train=X
        self.y_train=y
    def predict(self,X):
        
        if self.use_KDTree==False:
            matrix=np.zeros((len(self.X_train),2))
            for index, vector in enumerate (self.X_train):
                value=np.linalg.norm(vector-X)
                matrix[index,0]=self.y_train[index]
                matrix[index,1]=value
            sorted_matrix=matrix[matrix[:,1].argsort()]
            the_nearest=sorted_matrix[:self.n_neighbours]
            choose_Y=the_nearest[:,0]
            unique_class, counts =np.unique(choose_Y, return_counts=True)
            sorted_counts=unique_class[counts.argsort()[::-1]]
            
        else:
            X=np.array([X])
            kdtree=KDTree(self.X_train)
            _, indices =kdtree.query(X,self.n_neighbours)
            indices = indices.flatten()  
            choose_Y = self.y_train[indices]
            unique_class, counts =np.unique(choose_Y, return_counts=True)
            sorted_counts=unique_class[counts.argsort()[::-1]]
        return sorted_counts[0]
    def score(self,X,y):
        hit=0
        for i in range(len(X)):
            predict_y=self.predict(X[i])
            if predict_y==y[i]:
                hit+=1
          
        return hit/len(X) *100
#IMPLEMENTACJA DLA REGRESJI
class knn_regression(knn_classification):
    def __init__(self,n_neighbours=1,use_KDTree=False):
        super().__init__(n_neighbours,use_KDTree)
    def fit(self,X,y):
        super().fit(X,y)
    def predict(self,X):
         X=np.array([X])
         if self.use_KDTree==False:
            matrix=np.zeros((len(self.X_train),2))
            for index, vector in enumerate (self.X_train):
                value=np.linalg.norm(vector-X)
                matrix[index,0]=self.y_train[index]
                matrix[index,1]=value
            sorted_matrix=matrix[matrix[:,1].argsort()]
            the_nearest=sorted_matrix[:self.n_neighbours]
            choose_Y=the_nearest[:,0]
            predict_regression=np.sum(choose_Y)/len(choose_Y)
         else: 
         
            kdtree=KDTree(self.X_train)
            _, indices =kdtree.query(X,self.n_neighbours)
            indices = indices.flatten()  
            choose_Y = self.y_train[indices]
            predict_regression=np.mean(choose_Y)
         return predict_regression
    def score(self,X,y):
        msp=0
        for i in range(len(X)):
            predict_y=self.predict(X[i])
            suma=y[i]-predict_y
            msp+=suma**2
        return np.sqrt(msp/len(X))

            


    
#ZAD1,2,3
X,y =datasets.make_classification(
    n_samples=100,
    n_features=2,
    n_informative=2,
    n_redundant=0,
    n_repeated=0,
    random_state=3
    ) 

model=knn_classification(4,True)
model.fit(X,y)

h = 0.1
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                     np.arange(y_min, y_max, h))

Z = np.zeros(xx.shape)
for i in range(xx.shape[0]):
    for j in range(xx.shape[1]):
        Z[i, j] = model.predict([xx[i, j], yy[i, j]])

plt.figure(figsize=(10, 6))
plt.contour(xx, yy, Z, cmap=plt.cm.coolwarm)
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=plt.cm.coolwarm, edgecolors='k')
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.title("Granica decyzyjna k-NN (k=4)")
plt.show()
#ZAD4,5
#1
iris = datasets.load_iris()
X = iris.data
y=iris.target
model=knn_classification(4,True)
model.fit(X,y)

pca = PCA(n_components=2)
X_reduced = pca.fit(X).transform(X)
h = 0.2
x_min, x_max = X_reduced[:, 0].min() - 1, X_reduced[:, 0].max() + 1
y_min, y_max = X_reduced[:, 1].min() - 1, X_reduced[:, 1].max() + 1

xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                     np.arange(y_min, y_max, h))


Z = np.zeros(xx.shape)
for i in range(xx.shape[0]):
    for j in range(xx.shape[1]):
        point_2d=[xx[i, j], yy[i, j]]
        point_4d=pca.inverse_transform(point_2d)
        Z[i,j]=model.predict(point_4d)

custom_cmap = ListedColormap(['blue','green','red'])  
plt.figure(figsize=(10, 6))
plt.contour(xx, yy, Z, levels=[0,1], colors=['blue', 'green'])
plt.scatter(X_reduced[:, 0], X_reduced[:, 1], c=y, cmap=custom_cmap, edgecolors='k')
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.title("Granica decyzyjna k-NN po PCA(k=4)")
plt.show()
##ZAD6
n=[1,3,5,10,15,25,50,75,90]
for k in n:
    model=knn_classification(k,True)
    correct=0
    for i in range(X.shape[0]):
        X_train = np.delete(X,i,axis=0)
        y_train = np.delete(y,i,axis=0)
        model.fit(X_train,y_train)
        
        predict_point=model.predict(X[i])
        if predict_point==y[i]:
            correct+=1
    score=correct/X.shape[0]*100
    print(f"Dla n rowne: {k}. Dokladnosc to: {score:.2f} %")
#ZAD7

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.4, random_state=42)
n_values = [1, 15, 20]

for neighbors in n_values:
    start = time.time()
    model = knn_classification(neighbors, False)
    model.fit(X_train, y_train)
    score = model.score(X_test, y_test)
    stop = time.time()
    print(f"Dokladnosc dla n = {neighbors} bez KDTree to: {score:.2f} czas to: {(stop-start):.2f} sekund")

    start = time.time()
    model = knn_classification(neighbors, True)
    model.fit(X_train, y_train)
    score = model.score(X_test, y_test)
    stop = time.time()
    print(f"Dokladnosc dla n = {neighbors} z KDTree to: {score:.2f} czas to: {(stop-start):.2f} sekund")



#REGRESJA 4.1
X, y = datasets.make_regression(
    n_samples=100,
    n_features=2,
    noise=3,
    random_state=42
)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.4, random_state=42)
model=knn_regression(k,False)
model.fit(X_train, y_train)
score = model.score(X_test, y_test)
print(f"Średni błąd popełniany przez model: {score:.2f}")

model=knn_regression(k,False)
model.fit(X, y)
h = 0.2
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                     np.arange(y_min, y_max, h))

Z = np.zeros(xx.shape)
for i in range(xx.shape[0]):
    for j in range(xx.shape[1]):
        Z[i, j] = model.predict([xx[i, j], yy[i, j]])

plt.figure(figsize=(10, 6))
contour = plt.contourf(xx, yy, Z, cmap=plt.cm.coolwarm)
plt.colorbar(contour)
plt.scatter(X[:, 0], X[:, 1], c=y, cmap=plt.cm.coolwarm, edgecolors='k')
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.title("KNN dla regresji (k=4)")
plt.show()

#ZADANIE REGRESJA
boston=datasets.fetch_california_housing()
X_boston=boston.data[:200]
y_boston=boston.target[:200]
k_values = [1, 3, 5, 10, 15, 20]
kf = KFold(n_splits=10, shuffle=True, random_state=42)
results=[]
for k in k_values:
    scores=[]
    for train_index, test_index in kf.split(X_boston):
        X_train, X_test= X_boston[train_index], X_boston[test_index]
        y_train, y_test=y_boston[train_index], y_boston[test_index]
        model=knn_regression(k,True)
        model.fit(X_train,y_train)
        score=model.score(X_test,y_test)
        scores.append(score)
    mean_scores=np.mean(scores)
    results.append((k,mean_scores))

df_results = pd.DataFrame(results, columns=['k (liczba sąsiadów)', 'Średni RMSE'])
print(df_results)