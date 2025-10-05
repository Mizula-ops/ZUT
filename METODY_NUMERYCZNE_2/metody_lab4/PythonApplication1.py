import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import brentq
import time 

h=1e-5
def f1(x):
    return np.exp(x)+x**2-2

def sieczne(f,a,b,tol,ftol):
    a1=f(a)
    a2=(f(a+h)-2*f(a)+f(a-h))/h**2
    n=1
    x=[]
    if (a1 * a2) >=0:
        x.append(a)
        x.append(b)
    else:
        x.append(b)
        x.append(a)
    while True:
        new_x=x[n]-f(x[n])*(x[n]-x[n-1])/(f(x[n])-f(x[n-1]))
        x.append(new_x)
        n+=1  
        if abs(f(x[n])) < ftol or abs(x[n]-x[n-1]) <tol:
            break
    return x[n],n
    
def styczne(f,a,b,tol,ftol):
    a1=f(a)
    a2=(f(a+h)-2*f(a)+f(a-h))/h**2
    n=0
    x=[]
    if (a1 * a2) >=0:
        x.append(a)
    else:
        x.append(b)
    while True:
        new_x=x[n]-f(x[n])/(f(x[n]+h)-f(x[n]-h))/2*h
        x.append(new_x)
        n+=1  
        if abs(f(x[n])) < ftol or abs(x[n]-x[n-1]) <tol:
            break
    return x[n],n

def bisekcji(f,a,b,tol,ftol):
 
    n=0
    x=[]
    x.append(a)
    x.append(b)
    while True:
        new_x=(x[n]+x[n-1])/2
        x.append(new_x)
        n+=1  
        if x[n]-x[n-1] or abs(x[n]-x[n-1]) <tol:
            break
    return x[n],n
    

def wbudowana(f,a,b):
 
    result=brentq(f1,-1,2,1e-5,1e-4)
    return result

print("Rownanie 1")
        
start=time.time()
xb,n=sieczne(f1,1,3,1e-5,1e-4)
end=time.time()
print(f"Rozwiązanie za pomocą siecznych: {xb}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
start=time.time()
x= brentq(f1, a=-1, b=2, xtol=1e-5, rtol=1e-4, maxiter=100)
end=time.time()
print(f"Rozwiązanie za pomocą wbudowanej: {x}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
start=time.time()
xf,n=styczne(f1,1,3,1e-5,1e-4)
end=time.time()
print(f"Rozwiązanie za pomocą stycznej: {x}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
 

start=time.time()
xf,n=bisekcji(f1,1,3,1e-5,1e-4)
end=time.time()
print(f"Rozwiązanie za pomocą bisekcji: {x}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
  
t=np.arange(-1,3,0.01)
plt.plot(t,f1(t),label="f(x)")
plt.plot(xb, f1(xb), 'ro', label="Sieczne")
plt.plot(xf, f1(xf), 'rx', label="Styczne")
plt.title("Porównanie metod znajdowania miejsca zerowego")
plt.xlabel("x")
plt.ylabel("f(x)")
plt.legend()
plt.grid(True)
plt.show()