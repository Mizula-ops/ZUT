import numpy as np
import matplotlib.pyplot as plt
from numpy.ma.core import argmin, argmax
import random

def conflicts(board):
    n = len(board)
    c = 0
    for i in range(n):
        for j in range(i+1, n):
            if board[i] == board[j]:
                c += 1
            elif abs(board[i]-board[j]) == abs(i-j):
                c += 1
    return c

def evaluate(P):
    F=np.empty(P.shape[0])
    for i,board in enumerate(P):
        F[i]=conflicts(board)
    return F
def selection(P0, F, tournament_k=None):
    n = P0.shape[0]
    P = []
    i = 0
    while i < n:
        if tournament_k is None:
            tournamentpool = random.randint(1, n)
        else:
            tournamentpool = tournament_k
        tournament = []
        tournamentIndex = []
        for j in range(tournamentpool):
            while True:
                index = random.randint(0, n - 1)
                if index not in tournamentIndex:
                    tournamentIndex.append(index)
                    tournament.append(F[index])
                    break

        indexT = argmin(tournament)
        winner_index = tournamentIndex[indexT]
        P.append(P0[winner_index].copy())
        i += 1

    return np.array(P)

def cross(P1, P2):
    n = P1.shape[0]
    k = random.randint(1, n-1)
    c1 = np.concatenate([P1[:k], P2[k:]])
    c2 = np.concatenate([P2[:k], P1[k:]])
    return c1, c2

def crossover(P,pc):
    i=0
    while i<P.shape[0]-1:
        if random.random()<=pc:
            P[i],P[i+1]=cross(P[i],P[i+1])
        i=i+2
    return P
def mutation(P,pm):
    for i in range(P.shape[0]):
        for j in range(P.shape[1]):
            if random.random()<=pm:
                P[i,j]=random.randint(1,P.shape[1])
    return P
def replacement(P0, F0, P, F):
    bestF0 = argmin(F0)
    worstF = argmax(F)
    elite = P0[bestF0]
    exists = False
    for i in range(P.shape[0]):
        if np.array_equal(P[i], elite):
            exists = True
            break
    if not exists:
        P[worstF] = elite
        F[worstF] = F0[bestF0]
    return P, F


def evolution(P0, genmax, pc, pm,tournament_k=None):
    P = P0.copy()
    F = evaluate(P)

    best_history = []
    mean_history = []
    best_history.append(np.min(F))
    mean_history.append(np.mean(F))

    gen = 0
    bestFit = np.min(F)
    while gen < genmax and bestFit > 0:
        Pn = selection(P, F, tournament_k)
        Pn = crossover(Pn, pc)
        Pn = mutation(Pn, pm)
        Fn = evaluate(Pn)

        P, F = replacement(P, F, Pn, Fn)

        bestFit =np.min(F)
        best_history.append(bestFit)
        mean_history.append(np.mean(F))
        gen += 1

    best_idx = np.argmin(F)
    best_individual = P[best_idx].copy()
    best_fit = F[best_idx]

    return best_individual, best_fit, best_history, mean_history


def print_best_schematic(board):
    n = len(board)
    pos = [(col + 1, int(row)) for col, row in enumerate(board)]
    print("Pozycje hetmanów (kolumna, wiersz):")
    print(pos)
import csv

def run_baseline():
    genmax = 2000
    pc = 0.8
    pm = 0.05
    pop = 50
    seed = 0

    run_id = 1
    random.seed(seed)
    np.random.seed(seed)

    ns = [5, 10, 50, 100]

    csv_file = "baseline.csv"
    with open(csv_file, mode="w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow([
            "N",              # dodatkowo, żeby wiedzieć dla jakiego n
            "generation",
            "run",
            "parameter",
            "mean_fitness",
            "best_fitness"
        ])

        for n in ns:
            P0 = np.random.randint(1, n + 1, size=(pop, n))

            best_ind, best_fit, best_hist, mean_hist = evolution(P0, genmax, pc, pm)

            best_hist_clean = [int(x) for x in best_hist]
            mean_hist_clean = [float(x) for x in mean_hist]

            for gen in range(len(best_hist_clean)):
                writer.writerow([
                    n,
                    gen,
                    run_id,
                    "baseline",
                    mean_hist_clean[gen],
                    best_hist_clean[gen]
                ])

def experiment_population():
    import csv, random, numpy as np

    filename = "population.csv"
    with open(filename, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["generation", "run", "parameter", "mean_fitness", "best_fitness"])

        n = 50
        genmax = 2000
        pc = 0.8
        pm = 0.05
        runs = 5
        seed0 = 0

        pop_values = [20, 50, 100]

        for pop in pop_values:
            for run_id in range(1, runs + 1):
                seed = seed0 + run_id
                random.seed(seed); np.random.seed(seed)

                P0 = np.random.randint(1, n + 1, size=(pop, n))
                _, _, best_hist, mean_hist = evolution(P0, genmax, pc, pm)

                for g in range(len(best_hist)):
                    w.writerow([g, run_id, pop, float(mean_hist[g]), int(best_hist[g]),])
        w.writerow(["generation", "run", "parameter", "mean_fitness", "best_fitness"])

    print("Zapisano:", filename)
def experiment_generations():
    import csv, random, numpy as np

    filename = "generations.csv"
    with open(filename, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["generation", "run", "parameter", "mean_fitness", "best_fitness"])

        n = 50
        pop = 50
        pc = 0.8
        pm = 0.05
        runs = 5
        seed0 = 0

        gen_values = [500, 2000, 5000]

        for genmax in gen_values:
            for run_id in range(1, runs + 1):
                seed = seed0 + run_id
                random.seed(seed); np.random.seed(seed)

                P0 = np.random.randint(1, n + 1, size=(pop, n))
                _, _, best_hist, mean_hist = evolution(P0, genmax, pc, pm)

                for g in range(len(best_hist)):
                    w.writerow([g, run_id, genmax, float(mean_hist[g]), int(best_hist[g])])

    print("Zapisano:", filename)
def experiment_mutation():
    import csv, random, numpy as np

    filename = "mutation.csv"
    with open(filename, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["generation", "run", "parameter", "mean_fitness", "best_fitness"])

        n = 50
        pop = 50
        genmax = 2000
        pc = 0.8
        runs = 5
        seed0 = 0

        pm_values = [0.01, 0.05, 0.1]

        for pm in pm_values:
            for run_id in range(1, runs + 1):
                seed = seed0 + run_id
                random.seed(seed); np.random.seed(seed)

                P0 = np.random.randint(1, n + 1, size=(pop, n))
                _, _, best_hist, mean_hist = evolution(P0, genmax, pc, pm)

                for g in range(len(best_hist)):
                    w.writerow([g, run_id, pm, float(mean_hist[g]), int(best_hist[g])])

    print("Zapisano:", filename)

def experiment_crossover():
    import csv, random, numpy as np

    filename = "crossover.csv"
    with open(filename, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["generation", "run", "parameter", "mean_fitness", "best_fitness"])

        n = 50
        pop = 50
        genmax = 2000
        pm = 0.05
        runs = 5
        seed0 = 0

        pc_values = [0.6, 0.8, 0.95]

        for pc in pc_values:
            for run_id in range(1, runs + 1):
                seed = seed0 + run_id
                random.seed(seed); np.random.seed(seed)

                P0 = np.random.randint(1, n + 1, size=(pop, n))
                _, _, best_hist, mean_hist = evolution(P0, genmax, pc, pm)

                for g in range(len(best_hist)):
                    w.writerow([g, run_id, pc, float(mean_hist[g]), int(best_hist[g])])

    print("Zapisano:", filename)
def experiment_tournament():
    import csv, random, numpy as np

    filename = "tournament.csv"
    with open(filename, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["generation", "run", "parameter", "mean_fitness", "best_fitness"])

        n = 50
        pop = 50
        genmax = 2000
        pc = 0.8
        pm = 0.05
        runs = 5
        seed0 = 0

        k_values = [2, 3, 5, 10]

        for k in k_values:
            for run_id in range(1, runs + 1):
                seed = seed0 + run_id
                random.seed(seed); np.random.seed(seed)

                P0 = np.random.randint(1, n + 1, size=(pop, n))
                _, _, best_hist, mean_hist = evolution(P0, genmax, pc, pm, tournament_k=k)

                for g in range(len(best_hist)):
                    w.writerow([g, run_id, k, float(mean_hist[g]), int(best_hist[g])])

    print("Zapisano:", filename)

import numpy as np
import matplotlib.pyplot as plt

# Dane
sizes = np.array([64, 128, 256, 512, 1024, 2048, 4096])

opencl_gpu_float = np.array([0.0097149, 0.0015226, 0.0023086, 0.0124731, 0.100947, 0.954109, 7.70233])
opencl_gpu_int   = np.array([0.010281,  0.0015281, 0.002349,  0.0121314, 0.0956026, 0.936871, 7.475])

cpu_int   = np.array([0.0028589, 0.0013639, 0.0089825, 0.0865136, 2.45,    35.7078, 388.831])
cpu_float = np.array([0.0011442, 0.0039993, 0.0095683, 0.0837116, 2.90812, 39.5844, 420.981])

# Ustaw na True jeśli chcesz czytelniejszą skalę (duży rozstrzał czasów)
USE_LOG_Y = True

# =========================
# 1) Wykres kolumnowy: wszystkie technologie
# =========================
fig1, ax1 = plt.subplots(figsize=(11, 5))

x = np.arange(len(sizes))
w = 0.2

ax1.bar(x - 1.5*w, opencl_gpu_float, width=w, label="OpenCL GPU float")
ax1.bar(x - 0.5*w, opencl_gpu_int,   width=w, label="OpenCL GPU int")
ax1.bar(x + 0.5*w, cpu_int,          width=w, label="CPU int")
ax1.bar(x + 1.5*w, cpu_float,        width=w, label="CPU float")

ax1.set_xticks(x)
ax1.set_xticklabels([f"{s}x{s}" for s in sizes])
ax1.set_xlabel("Rozmiar macierzy")
ax1.set_ylabel("Czas mnożenia [s]")
ax1.set_title("Porównanie czasów mnożenia macierzy: wszystkie technologie")
ax1.grid(True, which="both", axis="y", linestyle="--", linewidth=0.5)
ax1.legend()

if USE_LOG_Y:
    ax1.set_yscale("log")

plt.tight_layout()


# =========================
# 2) Wykres liniowy: OpenCL GPU int vs float
# =========================
fig2, ax2 = plt.subplots(figsize=(11, 5))

ax2.plot(sizes, opencl_gpu_int, marker="o", label="OpenCL GPU int")
ax2.plot(sizes, opencl_gpu_float, marker="o", label="OpenCL GPU float")

ax2.set_xlabel("Rozmiar macierzy (N)")
ax2.set_ylabel("Czas mnożenia [s]")
ax2.set_title("OpenCL GPU: porównanie int vs float")
ax2.set_xscale("log", base=2)
ax2.grid(True, which="both", linestyle="--", linewidth=0.5)
ax2.legend()

if USE_LOG_Y:
    ax2.set_yscale("log")

plt.tight_layout()


# =========================
# 3) Wykres liniowy: CPU int vs float
# =========================
fig3, ax3 = plt.subplots(figsize=(11, 5))

ax3.plot(sizes, cpu_int, marker="o", label="CPU int")
ax3.plot(sizes, cpu_float, marker="o", label="CPU float")

ax3.set_xlabel("Rozmiar macierzy (N)")
ax3.set_ylabel("Czas mnożenia [s]")
ax3.set_title("CPU: porównanie int vs float")
ax3.set_xscale("log", base=2)
ax3.grid(True, which="both", linestyle="--", linewidth=0.5)
ax3.legend()

if USE_LOG_Y:
    ax3.set_yscale("log")

plt.tight_layout()
plt.show()

