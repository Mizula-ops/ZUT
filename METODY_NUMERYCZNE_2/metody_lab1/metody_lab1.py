import numpy as np
import matplotlib.pyplot as plt

#ZAD1
def f(x):
    return x * np.cos(x)

def metoda_prostokątów(a, b, n):
    h = (b - a) / n
    sum = 0
    for i in range(1, n+1):
        xi = a + (i - 1) * h
        sum += f(xi)
    return h * sum

# Metoda Trapezów
def metoda_trapezów(a, b, n):
    h = (b - a) / n
    sum = (f(a) + f(b)) / 2
    for i in range(1, n):
        xi = a + i * h
        sum += f(xi)
    return h * sum

# Metoda Simpsona
def metoda_simpsona(a, b, n):
    if n % 2 == 1:  
        n += 1
    h = (b - a) / n
    sum = f(a) + f(b)
    for i in range(1, n, 2):
        sum += 4 * f(a + i * h)
    for i in range(2, n-1, 2):
        sum += 2 * f(a + i * h)
    return (h / 3) * sum

# Metoda Monte Carlo
def metoda_monte_carlo(a, b, n):
    x_rand = np.random.uniform(a, b, n)
    f_rand = f(x_rand)
    return (b - a) * np.mean(f_rand)



fig, axes = plt.subplots(3, 3, figsize=(10, 12))


    x = np.linspace(0, 6, 100)
    df = f_pochodna(x)
    df_1 = z_definicji(f, x, h)
    df_3 = trojpunktowy(f, x, h)
    df_5 = pieciopunktowy(f, x, h)


   
    axes[0,i].plot(x, df_1, label='Z definicji', color='r')
    axes[0,i].plot(x, df, label='Dokładna pochodna', color='b')
    axes[0,i].set_title(f'Z definicji: h={h} blad : {mse_1pkt:.5f}')
    axes[0,i].legend()

 
    axes[1,i].plot(x, df_3, label='Trójpunktowa', color='r')
    axes[1,i].plot(x, df, label='Dokładna pochodna', color='b')
    axes[1,i].set_title(f'Trójpunktowy: h={h} blad : {mse_3pkt:.5f}')
    axes[1,i].legend()

   
    axes[2,i].plot(x, df_5, label='Pięciopunktowa', color='r')
    axes[2,i].plot(x, df, label='Dokładna pochodna', color='b')
    axes[2,i].set_title(f'Pięciopunktowy: h={h} blad : {mse_5pkt:.5f}')
    axes[2,i].legend()
#Wzor pieciopunktowy daje najlepszy reultat ponieważ uzwgedna najwiecej punktów
#Im mniejsze h tym większa dokładność
plt.tight_layout()
plt.show()

#ZAD2
import numpy as np
import matplotlib.pyplot as plt

def f_2(x):
    return np.sin(x) + np.cos(2 * x)

def f_pochodna_2(x):
    return np.cos(x) - 2 * np.sin(2 * x)

def forward(f, x, h):
    return (-3*f(x)+4*f(x+h)-f(x+2*h))/(2*h)

def central(f, x, h):
    return (f(x + h) - f(x - h)) / (2 * h)

def backward(f, x, h):
    return (f(x-2*h) - 4*f(x - h) +3*f(x)) / (2*h)




h_values_2 = [0.5, 0.25, 0.125]

fig, axes = plt.subplots(3, 3, figsize=(12, 12))

for i, h in enumerate(h_values_2):
    
    x_values_2 = np.linspace(0, 4, 100)
    df_forward = forward(f_2, x_values_2, h)
    df_central = central(f_2, x_values_2, h)
    df_backward = backward(f_2, x_values_2, h)
    
    df_2 = f_pochodna_2(x_values_2)
 
    mse_forward = blad_mse(df_forward, df_2)
    mse_central = blad_mse(df_central, df_2)
    mse_backward = blad_mse(df_backward, df_2)

    axes[0,i].plot(x_values_2, df_forward, label='Numeryczna (wprzód)', color='r')
    axes[0,i].plot(x_values_2, df_2, label='Dokładna', color='b')
    axes[0,i].set_title(f'Wprzód: h={h}, MSE={mse_forward:.5f}')
    axes[0,i].legend()

    axes[1,i].plot(x_values_2, df_central, label='Numeryczna (centralny)', color='r')
    axes[1,i].plot(x_values_2, df_2, label='Dokładna', color='b')
    axes[1,i].set_title(f'Centralny: h={h}, MSE={mse_central:.5f}')
    axes[1,i].legend()

    axes[2,i].plot(x_values_2, df_backward, label='Numeryczna (wstecz)', color='r')
    axes[2,i].plot(x_values_2, df_2, label='Dokładna', color='b')
    axes[2,i].set_title(f'Wstecz: h={h}, MSE={mse_backward:.5f}')
    axes[2,i].legend()

plt.tight_layout()
plt.show()
#Centralny najlepszy potem wstecz
#ZAD3
def f_3(x):
    return np.cos(4*x)-0.5*x
def f_druga_pochodna(x):
    return -16* np.cos(4*x)
def central_3pkt(f,x,h):
    return (f(x+h)-2*f(x)+f(x-h))/(h**2)
def central_5pkt(f,x,h):
    return (-f(x+2*h)+16*f(x+h)-30*f(x)+16*f(x-h)-f(x-2*h))/(12*h**2)
h_values_3 = [1, 0.5, 0.25]

fig, axes = plt.subplots(2, 3, figsize=(12, 12))

for i, h in enumerate(h_values_3):
  
    x_values_3 = np.linspace(0, 5, 100)
  
    df_central_3pkt = central_3pkt(f_3, x_values_3, h)
    df_central_5pkt= central_5pkt(f_3, x_values_3, h)
    
    df_3 = f_druga_pochodna(x_values_3)
 

    mse_central_3pkt = blad_mse(df_central_3pkt, df_3)
    mse_central_5pkt = blad_mse(df_central_5pkt, df_3)

    axes[0,i].plot(x_values_3, df_central_3pkt, label='Centralny trzypunktowy', color='r')
    axes[0,i].plot(x_values_3, df_3, label='Dokładna', color='b')
    axes[0,i].set_title(f'Trzypunktowy: h={h}, MSE={mse_central_3pkt:.5f}')
    axes[0,i].legend()
    
    axes[1,i].plot(x_values_3, df_central_5pkt, label='Centralny pięciopunktowy', color='r')
    axes[1,i].plot(x_values_3, df_3, label='Dokładna', color='b')
    axes[1,i].set_title(f'Pieciopunktowy: h={h}, MSE={mse_central_5pkt:.5f}')
    axes[1,i].legend()



plt.tight_layout()
plt.show()
#Metoda pięciopunktowa daje lepsze wyniki
#Zmiejszenie kroku h poprawia dokładność obliczeń