import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize
def f0(x,y):
    return 2*x**2+y**2+x*y-6*x-5*y+8
def pochodna(f, x, h=1e-5):
    return (f(x + h) - f(x - h)) / (2 * h)
def pochodna2(f, x, h=1e-5):
    return (f(x + h) - 2*f(x) + f(x - h)) / h**2
def gradient(f, x, y, h=1e-6):
    dfdx = (f(x + h, y) - f(x - h, y)) / (2 * h)
    dfdy = (f(x, y + h) - f(x, y - h)) / (2 * h)
    return dfdx, dfdy
def gradient_descent(f,x0,y0,xmin,xmax,ymin,ymax,krok=0.2,eps=0.1):
    x,y=x0,y0
    dx1=[]
    dx2=[]
    n=0
    while n <50:
        dfdx, dfdy = gradient(f, x, y)
        norma=np.sqrt(dfdx**2+dfdy**2)
        if norma<eps:
            break
        dfdx /= norma
        dfdy /= norma
        x -= krok * dfdx
        y -= krok * dfdy
        dx1.append(x)
        dx2.append(y)

        n += 1
    return x, y, dx1, dx2, n

xg,yg,dx1,dx2,n=gradient_descent(f0,-4,-4,-5,5,-5,5) 
res = minimize(lambda xy: f0(xy[0], xy[1]), [-4, -4], method='BFGS')
x_w, y_w = res.x

print(f"Dla funkcji :") 
print(f"  gradient  min_x = {xg:.10f}, min_y = {yg:.10f}, f = {f0(xg, yg):.10f}") 
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
contour=ax2.contour(X,Y,Z,levels=50, cmap="viridis")
ax2.plot(dx1,dx2,marker='o',linestyle='-')
ax2.scatter(xg,yg ,color='r',s=100,label="Minimum gauss")
fig.colorbar(contour,ax=ax2)
ax2.set_title("Wykres poziomicowy funckji f")
ax2.legend() 

plt.tight_layout()
plt.show()