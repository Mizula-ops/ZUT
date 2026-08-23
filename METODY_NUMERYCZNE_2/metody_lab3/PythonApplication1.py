import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import brentq
import time

h=1e-5
def f1(x):
    return np.exp(x-1)-2
def f2(x):
    return 2**-x + np.exp(x+2)*np.cos(x)-6
def f3(x):
    return np.log(x+2/3)
def f4(x):
    return x**3 - 2*x -5
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
        f_prime=(f(x[n]+h)-f(x[n]-h))/(2*h)
        new_x=x[n]-f(x[n])/f_prime
        x.append(new_x)
        n+=1  
        if abs(f(x[n])) < ftol or abs(x[n]-x[n-1]) <tol:
            break
    return x[n],n
    

def wbudowana(f,a,b):
 
    result=brentq(f1,-1,2,1e-5,1e-4)
    return result

def bisekcji(f,a,b,tol,ftol):
    n = 0
    while True:
        c = (a + b) / 2
        if abs(f(c)) < ftol or abs(b - a) < tol:
            break
        if f(a) * f(c) < 0:
            b = c 
        else:
            a = c 
        n += 1

    return c, n

for i in range(4):
    wybor =i
    if wybor==0:
        print("Rownanie 1")
        
        start=time.time()
        xb,n=sieczne(f1,-1,2,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą siecznych: {xb}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        x= brentq(f1, a=-1, b=2, xtol=1e-5, rtol=1e-4, maxiter=100)
        end=time.time()
        print(f"Rozwiązanie za pomocą wbudowanej: {x}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        xf,n=styczne(f1,-1,2,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą stycznej: {xf}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
       
        start=time.time()
        xg,n=bisekcji(f1,-1,2,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą bisekcji: {xg}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
       
        t=np.arange(-1,2,0.01)
        plt.plot(t,f1(t),label="f(x)")
        plt.plot(xb, f1(xb), 'ro', label="Sieczne")
        plt.plot(xf, f1(xf), 'rx', label="Styczne")
        plt.plot(xg, f1(xg), 'r+', label="Bisekcji")
        plt.title("Porównanie metod znajdowania miejsca zerowego")
        plt.xlabel("x")
        plt.ylabel("f(x)")
        plt.legend()
        plt.grid(True)
        plt.show()

    elif wybor==1:
        print("Rownanie 2")

        start=time.time()
        xb,n=sieczne(f2,1,3,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą siecznych: {xb}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        x= brentq(f2, a=1, b=3, xtol=1e-5, rtol=1e-4, maxiter=100)
        end=time.time()
        print(f"Rozwiązanie za pomocą wbudowanej: {x}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        xf,n=styczne(f2,1,3,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą stycznej: {xf}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        xg,n=bisekcji(f2,1,3,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą bisekcji: {xg}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
    
        t=np.arange(1,3,0.01)
        plt.plot(t,f2(t),label="f(x)")
        plt.plot(xb, f2(xb), 'ro', label="Sieczne")
        plt.plot(xf, f2(xf), 'rx', label="Styczne")
        plt.plot(xg, f2(xg), 'r+', label="Bisekcji")
        plt.title("Porównanie metod znajdowania miejsca zerowego")
        plt.xlabel("x")
        plt.ylabel("f(x)")
        plt.legend()
        plt.grid(True)
        plt.show()
    elif wybor==2:
        print("Rownanie 3")

        start=time.time()
        xb,n=sieczne(f3,-1/2,2,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą siecznych: {xb}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        x= brentq(f3, a=-1/2, b=2, xtol=1e-5, rtol=1e-4, maxiter=100)
        end=time.time()
        print(f"Rozwiązanie za pomocą wbudowanej: {x}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        xf,n=styczne(f3,-1/2,2,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą stycznej: {xf}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")

        start=time.time()
        xg,n=bisekcji(f3,-1/2,2,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą stycznej: {xg}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")

        t=np.arange(-1/2,2,0.01)
        plt.plot(t,f3(t),label="f(x)")
        plt.plot(xb, f3(xb), 'ro', label="Sieczne")
        plt.plot(xf, f3(xf), 'rx', label="Styczne")
        plt.plot(xg, f3(xg), 'r+', label="Bisekcji")
        plt.title("Porównanie metod znajdowania miejsca zerowego")
        plt.xlabel("x")
        plt.ylabel("f(x)")
        plt.legend()
        plt.grid(True)
        plt.show()

    elif wybor==3:
        print("Rownanie 4")

        start=time.time()
        xb,n=sieczne(f4,0,3,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą siecznych: {xb}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        x= brentq(f4, a=0, b=3, xtol=1e-5, rtol=1e-4, maxiter=100)
        end=time.time()
        print(f"Rozwiązanie za pomocą wbudowanej: {x}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")
        
        start=time.time()
        xf,n1=styczne(f4,0,3,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą stycznej: {xf}. Liczba iteracji: {n1}. Czas: {end-start:.5f} s")

        start=time.time()
        xg,n=bisekcji(f4,0,3,1e-5,1e-4)
        end=time.time()
        print(f"Rozwiązanie za pomocą bisekcji: {xg}. Liczba iteracji: {n}. Czas: {end-start:.5f} s")

        t=np.arange(0,3,0.01)
        plt.plot(t,f4(t),label="f(x)")
        plt.plot(xb, f4(xb), 'ro', label="Sieczne")
        plt.plot(xf, f4(xf), 'rx', label="Styczne")
        plt.plot(xg, f4(xg), 'r+', label="Bisekcji")
        plt.title("Porównanie metod znajdowania miejsca zerowego")
        plt.xlabel("x")
        plt.ylabel("f(x)")
        plt.legend()
        plt.grid(True)
        plt.show()
