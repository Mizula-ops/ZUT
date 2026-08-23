import pandas
import numpy as np
from scipy.sparse import csr_matrix
from sklearn.datasets import fetch_rcv1
from scipy.sparse import issparse
#ZAD1
def freq(x,prob=True):
    if issparse(x):
        x=x.toarray().flatten()
    lista={}
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

x=[1,2,1,1] 
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

print("Wartosci x:",x)
print("Wartosci y:",y)
xi, pi= freq2(x,y,prob=True)
xi1 ,ni= freq2(x,y,prob=False)
print("Unikalne wartosći xi: \n", xi)
print("Prawdopodobeinstwo pi: \n", pi)
print("Częstosci ni: \n", ni)

def entropy(x):
    _,pi=freq(x,prob=True)
    suma=0
    for p in pi:
        suma-=p*np.log2(p)
    return suma

def infogain(x,y):
    Hx=entropy(x)
    Hy=entropy(y)
    _,pi=freq2(x,y,prob=True)
    suma=0
    for p in pi:
        if p >0:
            suma-=p*np.log2(p)
    return Hx+ Hy -suma 

print("Wartosci x:",x)
print("Wartosci y:",y)
print("Entropia x:",entropy(x))
print("Entropia y:",entropy(y))
print("Przyrost informacji x,y",infogain(x,y))

#ZAD4
df = pandas.read_csv("zoo.csv") 
y=df["type"]

last_column=df.columns[-1]

korelacja=0;
slownik={}
for col in df.columns[1:-1]:
    korelacja=infogain(df[col],df[last_column])
    slownik[(col,last_column)]=korelacja

sorted_slownik =sorted(slownik.items(),key=lambda x: x[1], reverse=True)

for(col1,col2),korelacja in sorted_slownik:
    print(f"Przyrost informacji  {col1} a {col2}: {korelacja}")
  
data_x=np.array([1,2,1,1])
data_y=np.array([1,2,1,2])
kolumny=np.array([0,1,2,3])
wiersze =np.array([0,1,1,0])

sparse_matrix_x = csr_matrix((data_x,(kolumny,wiersze)),shape=(4,2)) 
sparse_matrix_y = csr_matrix((data_y,(kolumny,wiersze)),shape=(4,2)) 

xi,pi=freq(sparse_matrix_x,prob=True)
xi,ni=freq(sparse_matrix_x,prob=False)
print("Macierz x:")
print(sparse_matrix_x)
print("Unikalne wartosći: \n", xi)
print("Częstosci ni: \n", ni)
print("Prawdopodobienstwo pi: \n", pi) 

print("Macierz y:")
print(sparse_matrix_y)
xi,pi=freq2(sparse_matrix_x,sparse_matrix_y,prob=True)
xi,ni=freq2(sparse_matrix_x,sparse_matrix_y,prob=False)
print("Unikalne wartosći: \n", xi)
print("Częstosci ni: \n", ni)
print("Prawdopodobienstwo pi: \n", pi) 
#ZAD6
rcv1 = fetch_rcv1()
y = rcv1.target[:, 87]
x = rcv1["data"]

x.data = (x.data > 0).astype(int)

infogains = []

for i in range(10): 
    word = x[:, i]
    gain = infogain(word, y)
    infogains.append((i, gain))

sorted_infogains = sorted(infogains, key=lambda x: x[1], reverse=True)

print("50 słów dostarczających najwięcej informacji:")
for i, gain in sorted_infogains[:5]:
    print(f"Słowo {i} ma przyrost informacji: {gain:.8f}")