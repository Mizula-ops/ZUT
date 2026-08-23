from src.main import bfs, dfs
import time
import csv

def experiment():

    with open('result2.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["N","representation","algorithm","solution","open_count","closed_count","time"])
        for i in range (4,5):

            start = time.perf_counter()
            solution, closed,open_set= bfs(i,"board_perm")
            stop = time.perf_counter()
            elapsed = stop - start
            writer.writerow([i, "board_perm","bfs",solution,len(open_set),len(closed),format(elapsed, ".10f")])
            print("finisth board_perm bfs ",i)
            start = time.perf_counter()
            solution, closed, open_set = dfs(i, "board_perm")
            stop = time.perf_counter()
            elapsed = stop - start
            writer.writerow([i, "board_perm", "dfs", solution, len(open_set), len(closed), format(elapsed, ".10f")])
            print("finisth board_perm dfs ",i)
            start = time.perf_counter()
            solution, closed, open_set = bfs(i, "board_array")
            stop = time.perf_counter()
            elapsed = stop - start
            writer.writerow([i, "board_array", "bfs", solution, len(open_set), len(closed), format(elapsed, ".10f")])
            print("finisth board_array bfs ",i)
            start = time.perf_counter()
            solution, closed, open_set = dfs(i, "board_array")
            stop = time.perf_counter()
            elapsed = stop - start
            writer.writerow([i, "board_array", "dfs", solution, len(open_set), len(closed), format(elapsed, ".10f")])
            print("finisth board_array dfs ",i)
