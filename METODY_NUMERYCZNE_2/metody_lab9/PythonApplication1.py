import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize
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

def gauss_siedla(f,x0,y0,xmin, xmax, ymin, ymax,eps=1e-6):
    x, y = x0, y0
    dx1, dx2 = [], []
    n= 0
    dx1.append(x)
    dx2.append(y)
    while True:
       
        def fy(y_var):
            return f(x,y_var)
        y_new,_=zloty_podzial(fy,ymin,ymax)
        
        dx1.append(x)
        dx2.append(y_new)
        def fx(x_var):
            return f(x_var,y_new)
        
        x_new,_=zloty_podzial(fx,xmin,xmax)

        dx1.append(x_new)
        dx2.append(y_new)
        if np.sqrt((x_new-x)**2+(y_new-y)**2) <eps:
            return x_new,y_new,dx1,dx2,n
        x=x_new
        y=y_new
        n +=1

xg,yg,dx1,dx2,n=gauss_siedla(f0,-4,-4,-5,5,-5,5) 
res = minimize(lambda xy: f0(xy[0], xy[1]), [-4, -4], method='BFGS')
x_w, y_w = res.x

print(f"Dla funkcji :")
print(f"  gauss  min_x = {xg:.10f}, min_y = {yg:.10f}, f = {f0(xg, yg):.10f}")
print(f"  minimize min_x = {x_w:.10f}, min_y = {y_w:.10f}, f = {f0(x_w, y_w):.10f}")

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
ax2.plot(dx1,dx2,marker='o',linestyle='-')
ax2.scatter(xg,yg ,color='r',s=100,label="Minimum gauss")
fig.colorbar(contour,ax=ax2)
ax2.set_title("Wykres poziomicowy funckji f")
ax2.legend() 

plt.tight_layout()
plt.show()