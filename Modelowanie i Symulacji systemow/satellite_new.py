from scipy.integrate import RK45
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Circle
from math import sqrt


initial_condition = [
    0,  # x
    20, # Vx == x'
    5,  # y
    7   # Vy == y'
]

M = 200
R = 2
G = 9.81
rs = 0.2


def satellite(t, w):
    '''
    Definiowanie systemu równań różniczkowych

    Argumenty:
        w: wektor zmiennych stanu
            w = [x, x1, y, y1]
        t: time
    '''
    x, x1, y, y1 = w

    # f = [x', x1', y', y1']
    f = [ x1,
        (-1*G*M*x) / (x**2 + y**2) ** 1.5,
        y1,
        (-1*G*M*y) / (x**2 + y**2) ** 1.5 ]
    return f


def satellite_is_fallen(x, y):
    return sqrt(x**2 + y**2) <= R + rs


def simulate(t_min, t_max, max_step):
    '''
    Zwraca wartości wspórzędnych satelity x, y
    '''
    integrator = RK45(satellite,
                      t_min,
                      initial_condition,
                      t_max,
                      max_step=max_step)

    t = [t_min]
    x = [initial_condition[0]]
    y = [initial_condition[2]]

    while integrator.t < t_max:
        integrator.step()

        t.append(integrator.t)

        x.append(integrator.y[0])
        y.append(integrator.y[2])

        # Sprawdzamy czy satelita zdeżyła się z planetą.
        # Jeżeli tak to przerywamy symulacje.
        if satellite_is_fallen(x[-1], y[-1]):
            break

    return t, x, y


def scope(x, y):
    ax = plt.axes()

    lim = max(x+y) * 1.5
    axis = [ -lim, lim, -lim, lim ]
    ax.axis(axis)
    ax.set_aspect('equal')
    ax.plot(x, y)
    plt.title('Satellite trajectory')
    plt.show()


def visualization(x, y):
    fig, ax = plt.subplots(figsize=(5, 5), subplot_kw={'aspect': 'equal'}, dpi=150)

    xdata, ydata = [x[0]], [y[0]]

    # Plotujemy początkowe stany, zapisujemy do zmiennych
    sat, = plt.plot(x[0], y[0], 'ro')
    tra, = plt.plot(xdata, ydata, 'r')

    # Funkcja inicjacii animacji
    def init():
        lim = max(x+y) * 1.5
        axis = [ -lim, lim, -lim, lim ]
        ax.axis(axis)
        ax.set_aspect('equal')
        return sat, tra

    # Funckcja odświeżania animacji
    def update(frame):
        xdata.append(frame[0])
        ydata.append(frame[1])
        sat.set_data([frame[0]], [frame[1]])
        tra.set_data(xdata, ydata)
        return sat, tra

    # Dodajemy planete
    planet = Circle((0, 0), radius=R)
    ax.add_artist(planet)

    # Dodajemy legende
    ax.legend((sat, tra, planet),
              ('Satellite', 'Satellite trajectory', 'Planet'), ncols=2)

    # Tworzymy animacje używając poprzednie zdefiniowanych rzeczy
    ani = FuncAnimation(fig,
                        update,
                        frames=list(zip(x, y)),
                        init_func=init,
                        blit=True,
                        interval=8,
                        repeat=False)

    # Dodajemy tytuł i wyświetlamy
    plt.title('Visualization')
    plt.show()


def main():
    t, x, y = simulate(t_min = 0,
                       t_max = 10,
                       max_step = 2**-7)

    # scope(x, y)
    visualization(x, y)


if __name__=='__main__':
    main()

