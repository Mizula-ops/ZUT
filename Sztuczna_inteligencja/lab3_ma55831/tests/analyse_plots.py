##WIZUALIZACJA WYNIKOW w postaci wykresów
import matplotlib.pyplot as plt


def plot_metric(df, metric, title):

    df = df.copy()

    heuristics = df["heuristic"].unique()

    for heur in heuristics:
        if heur.lower() == "manhattan":
            continue
        subset = df[df["heuristic"] == heur]
        plt.plot(subset["N"],subset[metric],label=heur)

    plt.title(title)
    plt.xlabel("N – rozmiar planszy")
    plt.ylabel(metric.capitalize())

    plt.grid()

    plt.legend( title="Heurystyka",bbox_to_anchor=(1, 1),frameon=True)

    plt.tight_layout()
    plt.show()