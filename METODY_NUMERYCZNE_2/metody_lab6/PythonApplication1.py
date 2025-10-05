import numpy as np 
import matplotlib.pyplot as plt
from scipy.optimize import minimize


def f0(x1, x2):
    return x1**2 + x2**2

def f1(x1, x2):
    return -np.cos(x1) * np.cos(x2) * np.exp(-((x1 - np.pi)**2 + (x2 - np.pi)**2))

def f2(x1, x2):
    return (1 - 8*x1 + 7*x1**2 - (7/3)*x1**3 + (1/4)*x1**4) * (x2**2 * np.exp(-x2))

def f3(x1, x2):
    return -np.cos(x1) * np.cos(x2) * np.exp(-((x1 - np.pi)**2 + (x2 - np.pi)**2))

def f4(x1, x2):
    return -np.cos(x1) * np.cos(x2) * np.exp(-((x1 - np.pi)**2 + (x2 - np.pi)**2))


def siedla(f, x0, y0, xmin, xmax, ymin, ymax, krok=0.1, eps=1e-6):
    x, y = x0, y0
    dx1, dx2 = [], []
    iter_count = 0
    while True:
        x_old, y_old = x, y

             
        fx_plus = f(x + krok, y)
        fx_minus = f(x - krok, y)  
        if fx_plus < fx_minus and fx_plus < f(x, y):
            x += krok
        elif fx_minus < fx_plus and fx_minus < f(x, y):
            x -= krok

       
        fy_plus = f(x, y+krok)
        fy_minus = f(x, y-krok) 
        if fy_plus < fy_minus and fy_plus < f(x, y):
            y += krok
        elif fy_minus < fy_plus and fy_minus < f(x, y):
            y -= krok
        

        dx1.append(x)
        dx2.append(y)

        if abs(x - x_old) < eps and abs(y - y_old) < eps:
            break
    return x, y, dx1, dx2  


def ssw(f, x0, y0, xmin, xmax, ymin, ymax, krok=0.1, eps=1e-6):
    x, y = x0, y0
    dx1, dx2 = [], []
    iter_count = 0
    while True:
        x_old, y_old = x, y
        f_current = f(x, y)

        fx_plus = f(x + krok, y)
        fx_minus = f(x - krok, y) 
        if fx_plus < fx_minus and fx_plus < f_current:
            x += krok
            f_current = fx_plus
        elif fx_minus < fx_plus and fx_minus < f_current:
            x -= krok
            f_current = fx_minus
  
        
        fy_plus = f(x, y+krok)
        fy_minus = f(x, y-krok) 
        if fy_plus < fy_minus and fy_plus < f_current:
            y += krok
        elif fy_minus < fy_plus and fy_minus < f_current:
            y -= krok

        dx1.append(x)
        dx2.append(y)

        if abs(x - x_old) < eps and abs(y - y_old) < eps:
            break
    return x, y, dx1, dx2



func = [f0, f1, f2, f3, f4]
p_x = [8, 2, 5, 1.5, 1.5]
p_y = [-10, 2, 5, 5, 5]
x_min = [-10, 2, 0, -10, 1]
x_max = [10, 4, 5, 10, 5]
y_min = [-10, 2, 0, -10, 1]  
y_max = [10, 4, 5, 10, 5]
krok = [1, 0.1, 0.2, 0.5, 0.5]

for i in range(3):
    x_ssw, y_ssw, dx1_ssw, dx2_ssw = ssw(func[i], p_x[i], p_y[i], x_min[i], x_max[i], y_min[i], y_max[i], krok[i])
    x_si, y_si, dx1_si, dx2_si = siedla(func[i], p_x[i], p_y[i], x_min[i], x_max[i], y_min[i], y_max[i], krok[i])

    res = minimize(lambda xy: func[i](xy[0], xy[1]), [p_x[i], p_y[i]], method='BFGS')
    x_w, y_w = res.x

    print(f"Dla funkcji f{i}:")
    print(f"  ssw     min_x = {x_ssw:.10f}, min_y = {y_ssw:.10f}, f = {func[i](x_ssw, y_ssw):.10f}")
    print(f"  siedla  min_x = {x_si:.10f}, min_y = {y_si:.10f}, f = {func[i](x_si, y_si):.10f}")
    print(f"  minimize min_x = {x_w:.10f}, min_y = {y_w:.10f}, f = {func[i](x_w, y_w):.10f}")


    fig = plt.figure(figsize=(12,6))
    X, Y = np.meshgrid(np.linspace(x_min[i], x_max[i], 100), np.linspace(y_min[i], y_max[i], 100))
    Z = func[i](X, Y)

    ax1 = fig.add_subplot(1, 2, 1, projection="3d")
    ax1.plot_surface(X, Y, Z, cmap='viridis', alpha=0.8)
    ax1.set_title(f"Wykres 3D funkcji f{i}")

    min_x, min_y = x_si, y_si
    min_z = func[i](min_x, min_y)
    ax1.scatter(min_x, min_y, min_z, color='r', marker='o', s=100, label="Minimum ssw")
    ax1.legend()

    ax2 = fig.add_subplot(1, 2, 2)
    contour = ax2.contour(X, Y, Z, cmap='viridis')
    ax2.scatter(min_x, min_y, color='r', label="Minimum ssw", zorder=5)
    fig.colorbar(contour, ax=ax2)
    ax2.set_title(f"Wykres Poziomicowy funkcji f{i}")
    ax2.legend()

    plt.tight_layout()
    plt.show()
