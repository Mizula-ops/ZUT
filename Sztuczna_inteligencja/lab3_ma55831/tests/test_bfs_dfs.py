from src.main import BestFS, count_attacks, count_attacks_queens, manhattan
import time
import csv

def experiment():
    heuristic = [manhattan]
    representation = ["board_array","board_perm"]

    with open('result5.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["N","representation","heuristic","solution","open_count","closed_count","time"])
        for i in range (4,9):
            for repr in representation:
                for heur in heuristic:
                    if repr == "board_array" and (i == 7 or i == 8):
                        continue
                    start = time.perf_counter()
                    solution, closed,open_set= BestFS(i,repr,heur)
                    stop = time.perf_counter()
                    elapsed = stop - start
                    writer.writerow([i, repr,heur.__name__,solution,len(open_set),len(closed),format(elapsed, ".10f")])
                    print(f"finish dla {repr} z heurystyka {heur.__name__} czas {elapsed} dla {i}x{i} tablicy")



