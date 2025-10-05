import numpy as np
import matplotlib.pyplot as plt

#ZAD1

def wiPCA(X,n):
    mean=np.mean(X,axis=0)
    data=X-mean
    cov=np.transpose(data) @ data
    eig_values, eig_vectors= np.linalg.eigh(cov)
    sorted_index=np.argsort(eig_values)[::-1]
    sorted_eig_values=eig_values[sorted_index]
    sorted_eig_vectors=eig_vectors[:,sorted_index]
    final_vectors=sorted_eig_vectors[:,0:n]
    X_reduced = np.dot(final_vectors.T,data.T).T
    return X_reduced,sorted_eig_values,sorted_eig_vectors ,mean


#a)
X=np.random.randn(200,2)
A=np.array([[2,1],[1,1]])
X_transformed = X.dot(A)

plt.scatter(X_transformed[:,0],X_transformed[:,1],color="green")

new_X,values ,vectors ,mean =wiPCA(X_transformed,1)
final_vectors=vectors[:,0:1]
X_reconstructed= new_X @ final_vectors.T +mean
plt.scatter(X_reconstructed[:,0],X_reconstructed[:,1],color="red")

scale=3
if vectors[0, 0] < 0:
    vectors[:, 0] *= -1
if vectors[0, 1] > 0:
    vectors[:, 1] *= -1
plt.quiver(mean[0], mean[1], vectors[0,0], vectors[1,0], color='black',scale=0.15, scale_units='xy', angles='xy', label='Pierwszy wektor własny')

plt.quiver(mean[0],mean[1], vectors[0,1], vectors[1,1], color='black',scale=0.5, scale_units='xy', angles='xy', label='Ostatni wektor własny')

plt.title("200 losowych obiektow 2D (z korelacja)")
plt.xlabel("Cecha 1")
plt.ylabel("Cecha 2")
plt.grid(True)
plt.show() 

#ZAD2
#NASZ ALGORYTM
import pandas as pd
import seaborn as sb
import matplotlib.pyplot as plt
url = "https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data"
data = pd.read_csv(url, names=['sepal length','sepal width','petal length','petal width','target'])

x = data.iloc[:,0:4]
target = data.iloc[:,4]
 
mat_reduced, _, _, _ = wiPCA(x , 2)
mat_reduced[:,0]*=-1

principal_df = pd.DataFrame(mat_reduced , columns = ['PC1','PC2'])
principal_df = pd.concat([principal_df , pd.DataFrame(target)] , axis = 1)

plt.title("Wizualizacja obiektow z bazy iris (ALGORYTM wiPCA)")
sb.scatterplot(data = principal_df , x = 'PC1',y = 'PC2' , hue = 'target' , s = 60 , palette= 'icefire') 
plt.show()

#GOTOWY ALGORYTM
from sklearn import datasets
from sklearn.decomposition import PCA
iris = datasets.load_iris()
X = iris.data
pca = PCA(n_components=2)
mat_reduced = pca.fit(X).transform(X)

principal_df = pd.DataFrame(mat_reduced , columns = ['PC1','PC2'])
principal_df = pd.concat([principal_df , pd.DataFrame(target)] , axis = 1)

plt.title("Wizualizacja obiektow z bazy iris (ALGORYTM SKLEARN)")
sb.scatterplot(data = principal_df , x = 'PC1',y = 'PC2' , hue = 'target' , s = 60 , palette= 'icefire') 
plt.show()
#ZAD3
#ZAD3 a),c)
#NASZ ALGORYTM 
digits = datasets.load_digits()
X = digits.data
target=digits.target

X_digits,sorted_eig_values ,sorted_eig_vectors ,mean =wiPCA(X,2)
X_digits[:,1]*=-1
X_digits_df = pd.DataFrame(X_digits, columns=['PC1', 'PC2'])
X_digits_df['target'] = target
plt.figure(figsize=(12, 6))
plt.subplot(1,2,1)
plt.title("ALGORYTM wiPCA")
sb.scatterplot(data = X_digits_df , x = 'PC1',y = 'PC2' , hue = 'target' , s = 60 , palette= 'Spectral',alpha=0.6) 

#GOTOWY ALGORYTM WIZUALIZACJA I IMPLEMENTACJA
pca = PCA(n_components = 2)
X_r = pca.fit(X).transform(X) 
X_r[:,1] *= -1
X_r_df = pd.DataFrame(X_r, columns=['PC1', 'PC2'])
X_r_df['target'] = target

plt.subplot(1,2,2)
plt.title("ALGORYTM SKLEARN")
sb.scatterplot(data = X_r_df , x = 'PC1',y = 'PC2' , hue = 'target' , s = 60 , palette= 'Spectral',alpha=0.6) 
plt.show()

ZAD3 b)
def wariancja(X,sorted_eig_values,n):
    fr=np.zeros((n))
    cumulative_variance=0
    for i, value in enumerate(sorted_eig_values):
        cumulative_variance+=value
        fr[i]=cumulative_variance/np.sum(sorted_eig_values)
    return fr
wariancje=wariancja(X_digits,sorted_eig_values,X.shape[1])
liczba_skladowych=np.arange(1,X.shape[1]+1,1)
plt.title("Wariancja składowych głownych")
plt.plot(liczba_skladowych,wariancje,color="blue")
plt.ylabel("skumulowane wariancje")
plt.xlabel("numer składowej")
plt.show()

ZAD3 d)
def odwrotna_PCA(X_reduced, eig_vectors, mean, k):
    final_vectors=eig_vectors[:,:k]
    return X_reduced @ final_vectors.T + mean
def blad_rekonstrukcji(X_org, X_rek):
    return np.mean(np.linalg.norm(X_org- X_rek, axis=1))

bledy=[]
for k in range(1, X.shape[1] + 1):
    X_reduced,values,vectors,mean = wiPCA(X,k)
    X_rek = odwrotna_PCA(X_reduced,vectors,mean,k)
    blad = blad_rekonstrukcji(X,X_rek)
    bledy.append(blad)

plt.figure(figsize=(8, 5))
plt.plot(liczba_skladowych, bledy, color='blue')
plt.title("Średni błąd rekonstrukcji vs liczba składowych")
plt.xlabel("Liczba składowych")
plt.ylabel("Średni błąd rekonstrukcji")
plt.grid(True)
plt.show()



#ZADANIE DLA AMBITNYCH

digits = datasets.load_digits()
X = digits.data
target=digits.target


for n in [2, 4, 10, 50]:
    X_digits, sorted_eig_values, sorted_eig_vectors, mean = wiPCA(X, n)
    X_reconstructed = odwrotna_PCA(X_digits, sorted_eig_vectors, mean, n)

    fig, axes = plt.subplots(1, 10, figsize=(10, 2))
    for i in range(10):
        axes[i].imshow(X_reconstructed[i,:].reshape(8, 8), cmap='gray')
        axes[i].axis('off')
    plt.suptitle(f"Rekonstrukcja cyfr dla {n} składowych")
    plt.show()









