
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize_scalar
import time
def pochodna(f, x, h=1e-5):
    return (f(x + h) - f(x - h)) / (2 * h)
def pochodna2(f, x, h=1e-5):
    return (f(x + h) - 2*f(x) + f(x - h)) / h**2
h=1e-5
def f0(x):
    return x**3 + x**2 -20*x

def f1(x):
    return x**2 -x
def f2(x):
    return 1/3*x**2 -13/7*x + 11
def f3(x):
    return x**4 - 12*x**3 +x +4
def f4(x):
    return -x**3 + 3*x**2 -3*x

def styczne(f, a, b):
    h = 1e-5
 
    n = 0
    a1=f(a)
    a2=(f(a+h)-2*f(a)+f(a-h))/h**2
    
    x=0
    if (a1 * a2) >=0:
        x=a
    else:
        x=b
    while True:
        f1x = (f(x + h) - f(x - h)) / (2 * h)
        f2x = (f(x + h) - 2 * f(x) + f(x - h)) / (h ** 2)
        x_new = x - f1x / f2x
        if abs(x_new - x) < 0.01:
            break
        x = x_new
        n += 1
    return x, n 
def newtona(f,a,b,h=1e-5):
    if pochodna(f, a, h) < 0 and pochodna(f, b, h) > 0:
            x = (a + b) / 2
            n = 0
            while abs(pochodna(f, x, h)) > h:
                x_poch = pochodna(f, x, h)
                x_poch2 = pochodna2(f, x, h)
                x = x - x_poch / x_poch2
                n += 1
            return x, n
    else:
        if f(a) < f(b):
            return a, 0
        else:
            return b, 0
def zloty_podzial(f, a, b,h=1e-4):
    if pochodna(f, a, h) < 0 and pochodna(f, b, h) > 0:
        n = 0
        x=0
        while abs(b - a) > h:
            r=(np.sqrt(5)-1)/2
            x1=b-r*(b-a)
            x2=a+r*(b-a)
            if f(x1)<f(x2):
                b=x2
            
            else:
                a=x1
            n += 1
        return (a+b)/2, n 
    else:
        if f(a) < f(b):
            return a, 0
        else:
            return b, 0
def polowienia(f, a, b, h=1e-4):
    if pochodna(f, a, h) < 0 and pochodna(f, b, h) > 0:
        n = 0
        while (b - a) > h:
          
            xm = (a + b) / 2
            x1 = a + (b-a)/4
            x2 = b - (b-a)/4

            f1 = f(x1)
            f2 = f(x2)

            if f1 < f2:
             b = xm
            else:
                if f(xm)<f(x2):
                    a=x1
                    b=x2
                else:
                    a=xm
            
          

            n += 1

        return xm, n
    else:
        if f(a) < f(b):
            return a, 0
        else:
            return b, 0

def powella(f,a,b,h=1e-5):
    if pochodna(f, a, h) < 0 and pochodna(f, b, h) > 0:
        x1=a
        x3=b
        x2=(x1+x3)/2
        n=0
        fa=0
        while x3-x1 > 1e-5 or abs(fa)< 1e-5:
            delta_k=x2-x1
            xm=x2-0.5*delta_k*(f(x3)-f(x1))/(f(x1)-2*f(x2)+f(x3))
            x2=xm
            x3=x2+delta_k/2
            x1=x2-delta_k/2
            fa=(f(x2+h)-2*f(x2)+f(x2-h))/h**2

            n+=1
        return x2,n
    else:
        if f(a) < f(b):
            return a, 0
        else:
            return b, 0


def davidona(f, a, b, h=1e-5):
    if pochodna(f, a, h) < 0 and pochodna(f, b, h) > 0:
        n = 0
        h = 1e-5
        while True:
            fpa = pochodna(f, a, h)
            fpb = pochodna(f, b, h)
            fa = f(a)
            fb = f(b)

            Z = (3 * (fa - fb)) / (b - a) + fpa + fpb
            Q_sq = Z**2 - fpa * fpb
            Q = np.sqrt(Q_sq)
            numerator = (fpb + Q - Z)
            denominator = (fpb - fpa + 2 * Q)
            xm = b - (numerator / denominator) * (b - a)
            fpxm = pochodna(f, xm, h)

            n += 1

            if abs(fpxm) < h:
                break

            if fpxm > 0:
                b = xm
            else:
                a = xm

        return xm, n
    else:
        if f(a) < f(b):
            return a, 0
        else:
            return b, 0

   
    
def newtona(f,a,b,h=1e-5):
    if pochodna(f, a, h) < 0 and pochodna(f, b, h) > 0:
            x = (a + b) / 2
            n = 0
            while abs(pochodna(f, x, h)) > h:
                x_poch = pochodna(f, x, h)
                x_poch2 = pochodna2(f, x, h)
                x = x - x_poch / x_poch2
                n += 1
            return x, n
    else:
        if f(a) < f(b):
            return a, 0
        else:
            return b, 0
f=[f0,f1,f2,f3,f4]
a=[0,1/4,-10,-2,-2]
b=[6,3/4,10,2,-1] 
for i in range(5):
        print(f"WYNIKI DLA FUNCKJI {i}")
        xb= minimize_scalar(f[i],method='bounded', bounds=(a[i],b[i]),options={'xatol':0.01})
        print(f"Rozwiązanie za pomocą Wbudowanej: {xb.x}. Liczba iteracji: {xb.nit}.")
        
        xp1,n=polowienia(f[i],a[i],b[i])
        print(f"Rozwiązanie metoda Polowienia: {xp1}. Liczba iteracji: {n}.")
 
        xz,n=zloty_podzial(f[i],a[i],b[i])
        print(f"Rozwiązanie metoda Zlotego podzialu: {xz}. Liczba iteracji: {n}.")

        xp2,n=powella(f[i],a[i],b[i])
        print(f"Rozwiązanie metoda Powella: {xp2}. Liczba iteracji: {n}.")

        xd,n=davidona(f[i],a[i],b[i])
        print(f"Rozwiązanie metoda Davidona: {xd}. Liczba iteracji: {n}.")

        xe,n=newtona(f[i],a[i],b[i])
        print(f"Rozwiązanie metoda newtona: {xe}. Liczba iteracji: {n}.")

        x_min=[xb.x,xp1,xz,xp2,xd,xe]
        colors=["blue","pink","red","green","orange","yellow"]
        labels = ["wbudowana", "Połowienie", "Złoty podział","powella","dawvidona","newtona"]
        markers = ["o", "s", "D", "^","v","x"]
        x=np.linspace(a[i],b[i],1000)
        plt.plot(x,f[i](x),color="orange")
        plt.xlabel("x")
        plt.ylabel("y")
        plt.title("znajdowanie minimum")
        for j in range(6):
            plt.scatter(x_min[j],f[i](x_min[j]),color=colors[j],label=labels[j],marker=markers[j])
        plt.legend()
        plt.grid()
        plt.show()