import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize
def f0(x,y):
    return 2*x**2+y**2+x*y-6*x-5*y+8
def pochodna(f, x, h=1e-5):
    return (f(x + h) - f(x - h)) / (2 * h)
def pochodna2(f, x, h=1e-5):
    return (f(x + h) - 2*f(x) + f(x - h)) / h**2
def ssw(f, x0, y0, xmin, xmax, ymin, ymax, krok=0.2, eps=1e-6):
    x, y = x0, y0
    dx1, dx2 = [], []
    iter_count = 0
    while True:
        x_old, y_old = x, y
     
        fx_plus = f(x + krok, y)
        fx_minus = f(x - krok, y) 
        fy_plus = f(x, y+krok)
        fy_minus = f(x, y-krok) 
        fx=f(x,y)
        wartosci=[fx,fx_plus,fx_minus,fy_plus,fy_minus]
        min_index=wartosci.index(min(wartosci))

        if min_index == 0:    
            x, y = x, y
        elif min_index == 1:    
            x, y = x + krok, y
        elif min_index == 2:    
            x, y = x - krok, y
        elif min_index == 3:
            x, y = x, y + krok
        elif min_index == 4:    
            x, y= x, y - krok

        dx1.append(x)
        dx2.append(y)

        if abs(x - x_old) < eps and abs(y - y_old) < eps:
            break
    return x, y, dx1, dx2 

x_ssw, y_ssw, dx1_ssw, dx2_ssw = ssw(f0, -4, -4, -5, 5, -5, 5)
res = minimize(lambda xy: f0(xy[0], xy[1]), [0, 0], method='BFGS')
x_w, y_w = res.x

print(f"Dla funkcji f:")
print(f"  ssw     min_x = {x_ssw:.10f}, min_y = {y_ssw:.10f}, f = {f0(x_ssw, y_ssw):.10f}")

print(f"  minimize min_x = {x_w:.10f}, min_y = {y_w:.10f}, f = {f0(x_w, y_w):.10f}")

fig = plt.figure(figsize=(12,6))
X,Y=np.meshgrid(np.linspace(-5,5,100),np.linspace(-5,5,100))
Z=f0(X,Y) 

ax1=fig.add_subplot(1,2,1, projection="3d")
ax1.plot_surface(X,Y,Z, cmap='viridis', alpha=0.8)
ax1.set_title(f"Wykres 3D funckji")
ax1.scatter(x_ssw,y_ssw,f0(x_ssw,y_ssw) ,color='r',s=100,label="Minimum gauss")
ax1.legend() 

ax2=fig.add_subplot(1,2,2)
contour=ax2.contour(X,Y,Z, cmpa="viridis")
ax2.plot(dx1_ssw,dx2_ssw,marker='o',linestyle='-')
ax2.scatter(x_ssw,y_ssw ,color='r',s=100,label="Minimum gauss")
fig.colorbar(contour,ax=ax2)
ax2.set_title("Wykres poziomicowy funckji f")
ax2.legend() 

plt.tight_layout()
plt.show()