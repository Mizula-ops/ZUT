import numpy as np
import matplotlib.pyplot as plt
def f0(x,y):
    return 2*x**2+y**2+x*y-6*x-5*y+8
def pochodna(f, x, h=1e-5):
    return (f(x + h) - f(x - h)) / (2 * h)
def pochodna2(f, x, h=1e-5):
    return (f(x + h) - 2*f(x) + f(x - h)) / h**2
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

def gauss_siedla(f,x0,y0,xmin, xmax, ymin, ymax, krok=0.1, eps=1e-6):
    x, y = x0, y0
 
    n= 0
    while True:
        def fx(x_var):
            return f(x_var,y)
        def fy(y_var):
            return f(x,y_var)
        x_new,_=zloty_podzial(fx,xmin,xmax)
        y_new,_=zloty_podzial(fy,ymin,ymax)
        
        if np.sqrt((x_new-x)**2+(y_new-y)**2) <eps:
            return x_new,y_new,n
       
 
        x=x_new
        y=y_new
        n +=1
def pochodna(f, x, h=1e-5):
    return (f(x + h) - f(x - h)) / (2 * h)
def gradient(f,x0,y0,xmin,xmax,ymin,ymax,krok=0.1,eps=1e-6):
    x,y=x0,y0
    n=0
    while True:
        x_old,y_old=x,y
        def fx(x_var):
            return f(x_var, y)
        def fy(y_var):
            return f(x, y_var)
        dfdx=pochodna(fx,x)
        dfdy=pochodna(fy,y)
        gradf=np.array([dfdx,dfdy])
        norma=np.linalg.norm(gradf)
        x_new, y_new = np.array([x, y]) - krok * gradf / norma
        x = min(max(x_new, xmin), xmax)
        y = min(max(y_new, ymin), ymax)
        if norma<eps:
            break

        n += 1
    return x,y,n
xg,yg,n=gauss_siedla(f0,0,0,-5,5,-5,5)
xgr,ygr,n=gradient(f0,0,0,-5,5,-5,5)
print(f"Dla funkcji :")
print(f"  gauss  min_x = {xg:.10f}, min_y = {yg:.10f}, f = {f0(xg, yg):.10f}")
print(f"  gradient  min_x = {xgr:.10f}, min_y = {ygr:.10f}, f = {f0(xgr, ygr):.10f}")
fig = plt.figure(figsize=(12,6))
X,Y=np.meshgrid(np.linspace(-5,5,100),np.linspace(-5,5,100))
Z=f0(X,Y) 

ax1=fig.add_subplot(1,2,1, projection="3d")
ax1.plot_surface(X,Y,Z, cmap='viridis', alpha=0.8)
ax1.set_title(f"Wykres 3D funckji")
ax1.scatter(xg,yg,f0(xg,yg) ,color='r',s=100,label="Minimum gauss")
ax1.legend() 

ax2=fig.add_subplot(1,2,2)
contour=ax2.contour(X,Y,Z, cmpa="viridis")
ax2.scatter(xg,yg ,color='r',s=100,label="Minimum gauss")
fig.colorbar(contour,ax=ax2)
ax2.set_title("Wykres poziomicowy funckji f")
ax2.legend() 

plt.tight_layout()
plt.show()