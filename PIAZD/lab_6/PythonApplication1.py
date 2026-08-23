import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from matplotlib.patches import Circle
import time
# Załaduj dane
df = pd.read_csv("autos.csv") 
df = df.fillna(0)
df_5 = df.head(3)
df_selected=df[["horsepower","engine-size"]]

X = df.select_dtypes(include=[np.number]).values 
def distp(x,c):
    return np.sqrt(np.dot((x-c),(x-c).T))
def distm(x,c):
    V_inv=distp(x,c)
    first_arg=np.dot(x-c,V_inv)
    return np.sqrt(np.dot(first_arg,(x-c).T))
def k_srodki(x, k,which):
    n, m = x.shape
    P = np.zeros((n, k))
    C = np.zeros((k, m))
    for i in range(k):
        losowa_liczba = np.random.randint(0, n)
        C[i, :] = x[losowa_liczba, :]
        np.set_printoptions(precision=4)
    while True:
        P_new = np.zeros((n, k))
        lista = np.zeros(k)
        for j in range(n):
            for i in range(k):
                if which==True:
                    de=distp(x[j,:],C[i,:])
                else:
                    de=distm(x[j,:],C[i,:])
                lista[i] = de

            min_indices = np.where(lista == np.min(lista))[0]
            min_index = np.random.choice(min_indices)
            P_new[j, min_index] = 1
        #print(f"Przypisania punktów: {P_new.sum(axis=0)}")  
        if np.array_equal(P_new, P):
            break
        P = P_new.copy()
        
        C_old = C.copy()
        for i in range(k):
            mianownik = np.sum(P[:, i])
            suma = np.zeros(m)
            for j in range(n):
                if P[j, i] == 1:
                    suma += x[j, :]
            if mianownik > 0:
                C[i, :] = suma / mianownik
            #print(f"Punkt centralny {i}: {C[i,:]}")
        if np.array_equal(C, C_old):
            break    
    return P, C
def oblicz_F(k,C,P,x):
    gora=0
    dol=0
    for i in range(k-1):
        for j in range(i+1,k):
            center=distp(C[i,:],C[j,:])
            gora+=center
    for i in range(k):
        for j in range(len(x)):
            if P[j,i]==1:
                dol+=distp(x[j,:],C[i,:])**2
    return gora/dol 



P, C = k_srodki(X, 3,False)
n,m = X.shape
radius=np.zeros((3,n))
radius_sum=np.zeros(3)

for i in range(3):
    max_distance=0
    for j in range(n):
        if P[j,i]==1:
            dist=distp(X[j,:],C[i,:])
            if dist>max_distance:
                max_distance=dist
    radius_sum[i]=max_distance

colors = ["hotpink", "blue", "orange"]
markers = ["o", "s", "^"]
labels = ["Grupa 1", "Grupa 2", "Grupa 3"]


for i in range(3):  
    grupa = X[P[:, i] == 1]
    plt.scatter(grupa[:, 0], grupa[:, 1], color=colors[i], marker=markers[i], label=labels[i], alpha=0.6)
    circle=plt.Circle((C[i, 0], C[i, 1]),radius_sum[i],color=colors[i],fill=False,linestyle="--")
    plt.gca().add_artist(circle)

plt.scatter(C[:, 0], C[:, 1],color="black", marker="X", s=100, label="Centroidy")



plt.xlabel("Horsepower")
plt.ylabel("engine-size")
plt.legend()
plt.title("Wizualizacja grup po k-środkach")
plt.grid(True)

plt.show()


wynik=oblicz_F(3,C,P,X)
print(f"Jakosc grupowania: {wynik:.16f}")

df = pd.read_csv("autos.csv") 
df = df.fillna(0)
X = df.select_dtypes(include=[np.number]).values 
k=[2,3,4,5]
np.random.seed(int(time.time()))
for i in k:
    for j in range(5):
        P, C = k_srodki(X, i,True)
        wynik_Eukli=oblicz_F(i,C,P,X)
        print(f"Wynik dla k = {i}. Metryka Euklidesowa. Jakość: {wynik_Eukli:.16f}")
        P, C = k_srodki(X, i,False)
        wynik_Mahalan=oblicz_F(i,C,P,X)
        print(f"Wynik dla k = {i}. Metryka Mahalanobisa. Jakość: {wynik_Mahalan:.16f}")
