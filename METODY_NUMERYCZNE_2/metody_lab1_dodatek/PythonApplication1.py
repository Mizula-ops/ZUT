import numpy as np
import matplotlib.pyplot as plt

#ZADANIE DODATKOWE
def f(x):
    return x**np.cos(x)

def f_pochodna(x):
    return ((np.cos(x)/x)-np.sin(x)*np.log(x))*x**np.cos(x)

def z_definicji(f, x, h):
    return (f(x+h)-f(x))/h

def trojpunktowy(f, x, h):
    return (f(x+h)-f(x-h))/(2*h)  

def pieciopunktowy(f, x, h):
    return (-f(x+2*h)+8*f(x+h)-8*f(x-h)+f(x-2*h))/(12*h)

def blad_mse(d, p):
    n = len(d)
    return np.sum((d-p)**2)/n


h = 0.2
fig, axes = plt.subplots(figsize=(10, 12))
x = np.linspace(0, 10, 100)
df = f_pochodna(x)
df_1 = z_definicji(f, x, h)
df_3 = trojpunktowy(f, x, h)
df_5 = pieciopunktowy(f, x, h)

mse_1pkt = blad_mse(df, df_1)
mse_3pkt = blad_mse(df, df_3)
mse_5pkt = blad_mse(df, df_5)

axes.plot(x, df, label='Pochodna Analityczna', color='b')
axes.plot(x, df_1, label='Z definicji', color='r')
axes.plot(x, df_3, label='Trójpunktowa', color='g')
axes.plot(x, df_5, label='Pięciopunktowa', color='c')
axes.set_title('')
axes.legend()
plt.tight_layout()
plt.show()