
import pandas
import numpy as np
from scipy.sparse import csr_matrix
from sklearn.datasets import fetch_rcv1
from scipy.sparse import issparse
#ZAD1
def freq(x,prob=True):
    lista={} 
    if issparse(x):
        x=x.toarray().flatten()

    for el in x:
        for xi in lista:
            if el == xi:
                break
        else:
            lista[el]=0
    for el in x:
        lista[el]+=1
    xi=list(lista.keys())
    ni=list(lista.values())
    pi=[]
    if prob:
        total=len(x)
        for n in ni:
            pi.append(n/total)
        return xi,pi
    else:
        return xi,ni


x=[1,2,2,3,1,2]



xi,pi= freq(x,prob=True)
xi1,ni= freq(x,prob=False)
print("Unikalne wartosći xi: \n", xi)
print("Częstosci ni: \n", ni)
print("Prawdopodobienstwo pi: \n", pi)
#ZAD2
def freq2(x,y, prob=True):  
    lista={}
    if issparse(x):
        x=x.toarray().flatten()
    if issparse(y):
        y=y.toarray().flatten()
    for xi,yi in zip(x,y):
        pair =(xi,yi)
        if pair not in lista:
            lista[pair]=0
    
    for xi,yi in zip(x,y):
        pair =(xi,yi)
        lista[pair]+=1
    xi=list(lista.keys())
    ni=list(lista.values())
    pi=[]
    if prob:
        total=len(x)
        for n in ni:
            pi.append(n/total)
        return xi,pi
    else:
        return xi,ni


x=[1,2,1,1]
y=[1,0,1,0]

xi, pi= freq2(x,y,prob=True)
xi1 ,ni= freq2(x,y,prob=False)
print("Unikalne wartosći xi: \n", xi)
print("Prawdopodobeinstwo pi: \n", pi)
print("Częstosci ni: \n", ni)

def entropy(x):
    _,pi=freq(x,prob=True)
    suma=0
    for p in pi:
        suma+= -p*np.log2(p)
    return suma

def infogain(x,y):
    Hx=entropy(x)
    Hy=entropy(y)
    total=len(x)
    _,pi=freq2(x,y,prob=True)
    suma=0
    for p in pi:
        suma+=-p*np.log2(p)
    return Hx+ Hy - suma
   
print(entropy(x))
print(entropy(y))
print(infogain(x,y)) 
#ZAD4
df = pandas.read_csv("zoo.csv") 
test=pandas.read_csv("zoo.csv")
y=df["type"]

last_column=df.columns[-1]

korelacja=0;
slownik={}
for col in df.columns[1:-1]:
    korelacja=infogain(df[col],df[last_column])
    slownik[(col,last_column)]=korelacja
    print(f"Korelacja między {col} a {last_column}: {korelacja}")

print(slownik)
sorted_slownik =sorted(slownik.items(),key=lambda x: x[1], reverse=True)

for(col1,col2),korelacja in sorted_slownik:
    print(f"Korelacja między {col1} a {col2}: {korelacja}")
     

#ZAD5
def freq_csr(x,prob=True):
    lista={}
    
    for el in x.data:
        for xi in lista:
            if el == xi:
                break
        else:
            lista[el]=0
    for el in x.data:
        lista[el]+=1
    xi=list(lista.keys())
    ni=list(lista.values())
    pi=[]
    if prob:
        total=x.shape[0]
        for n in ni:
            pi.append(n/total)
        return xi,pi
    else:
        return xi,ni

def freq3_csr(x,y, prob=True):  
    lista={}
  
    x = x.toarray().flatten()
    
    y = y.toarray().flatten()
    for xi,yi in zip(x.data,y.data):
        pair =(xi,yi)
        if pair not in lista:
            lista[pair]=0
    
    for xi,yi in zip(x.data,y.data):
        pair =(xi,yi)
        lista[pair]+=1
    xi=list(lista.keys())
    ni=list(lista.values())
    pi=[]
    if prob:
        total=len(x.data)
        for n in ni:
            pi.append(n/total)
        return xi,pi
    else:
        return xi,ni


def freq2_csr(xy, prob=True):  
    lista={}
    for xi,yi in zip(xy.data,xy.indices):
        pair =(xi,yi)
        if pair not in lista:
            lista[pair]=0
    
    for xi,yi in zip(xy.data,xy.indices):
        pair =(xi,yi)
        lista[pair]+=1
    xi=list(lista.keys())
    ni=list(lista.values())
    pi=[]
    if prob:
        total=len(xy.data)
        for n in ni:
            pi.append(n/total)
        return xi,pi
    else:
        return xi,ni

def entropy_csr(x):
    _,pi=freq_csr(x,prob=True)
    suma=0
    for p in pi:
        suma+= -p*np.log2(p)
    return suma

def infogain_csr(x,y):
    Hx=entropy_csr(x)
    Hy=entropy_csr(y)
    total=len(x.data)
    _,pi=freq3_csr(x,y,prob=True)
    suma=0
    for p in pi:
        suma+=-p*np.log2(p)
    return Hx+ Hy - suma

data=np.array([4,2,3])
kolumny=np.array([1,2,2])
wiersze =np.array([1,3,1])

sparse_matrix = csr_matrix((data,(kolumny,wiersze)),shape=(3,4)) 


xi,pi=freq_csr(sparse_matrix,prob=True)
xi,ni=freq_csr(sparse_matrix,prob=False)
print("Unikalne wartosći xi88: \n", xi)
print("Częstosci ni: \n", ni)
print("Prawdopodobienstwo pi: \n", pi) 
xi,pi=freq(sparse_matrix,prob=True)
xi,ni=freq(sparse_matrix,prob=False)
print("Unikalne wartosći xi99: \n", xi)
print("Częstosci ni: \n", ni)
print("Prawdopodobienstwo pi: \n", pi) 

xi,pi=freq2_csr(sparse_matrix,prob=True)
xi,ni=freq2_csr(sparse_matrix,prob=False)
print("Unikalne wartosći xi: \n", xi)
print("Częstosci ni: \n", ni)
print("Prawdopodobienstwo pi: \n", pi) 
from sklearn.feature_extraction.text import TfidfVectorizer
rcv1 = fetch_rcv1()
y = rcv1.target[:, 87]
x = rcv1["data"]

x.data = (x.data > 0).astype(int)

infogains = []

for i in range(5):  # x.shape[1] to liczba unikalnych słów (cech)
    word = x[:, i]
    gain = infogain_csr(word, y)
    infogains.append((i, gain))

sorted_infogains = sorted(infogains, key=lambda x: x[1], reverse=True)

print("50 słów dostarczających najwięcej informacji:")
for i, gain in sorted_infogains[:5]:
    print(f"Słowo {i} ma przyrost informacji: {gain}")