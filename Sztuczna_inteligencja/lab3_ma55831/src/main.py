from sys import exec_prefix

import numpy as np
import heapq
import pandas as pd

def conflicts(board):
    my_array=np.array(board)
    n=my_array.shape[0]
    help_array = [-1] * n
    if my_array.ndim == 2:
        for r in range(n):
            for c in range(n):
                if my_array[r][c] == 1:
                    if help_array[r] !=-1 or  c in help_array:
                        return False
                    help_array[r]=c
        if -1 in help_array:
            return False
    else:
        if -1 in my_array:
            return False
        help_array=my_array
    for i in range(n):
        for j in range(i+1,n):
            if abs(help_array[i]-help_array[j]) == abs(i-j):
                return False
    return True

def count_attacks(board):
    my_array=np.array(board)
    n=my_array.shape[0]
    count=0
    if my_array.ndim == 2:
        hetmans=[(r,c) for r in range(n) for c in range(n) if my_array[r][c]==1]
        for i in range(len(hetmans)):
            r1,c1=hetmans[i]
            for j in range(i+1,len(hetmans)):
                r2,c2=hetmans[j]
                if r1==r2 or c1 == c2 or abs(r1-r2)==abs(c1-c2):
                    count+=1
    else:
        for i in range(n):
            if my_array[i] == -1:
                continue
            for j in range(i+1,n):
                if my_array[j] == -1:
                    continue
                if abs(my_array[i]-my_array[j]) == abs(i-j):
                    count+=1
    return count

def count_attacks_queens(board):
    my_array=np.array(board)
    n=my_array.shape[0]
    count=0
    count_hetmans=0
    if my_array.ndim == 2:
        hetmans = [(r, c) for r in range(n) for c in range(n) if my_array[r][c] == 1]
        for i in range(len(hetmans)):
            r1,c1=hetmans[i]
            for j in range(i+1,len(hetmans)):
                r2,c2=hetmans[j]
                if r1==r2 or c1 == c2 or abs(r1-r2)==abs(c1-c2):
                    count+=1
        count_hetmans=len(hetmans)

        return count*2 + n-count_hetmans
    else:
        for i in range(n):
            if my_array[i]==-1:
                continue
            count_hetmans += 1
            for j in range(i+1,n):
                if my_array[j]==-1:
                    continue
                if abs(my_array[i]-my_array[j]) == abs(i-j):
                    count+=1
    return count + n-count_hetmans

def manhattan(board):
    my_array=np.array(board)
    n=my_array.shape[0]
    manhattan_distance=0
    if my_array.ndim == 2:
        hetmans=[(r,c) for r in range(n) for c in range(n) if my_array[r][c]==1]
        for i in range(len(hetmans)):
            r1,c1=hetmans[i]
            for j in range(i+1,len(hetmans)):
                r2,c2=hetmans[j]
                score=abs(r1-r2)+abs(c1-c2)
                if score <3:
                    score=n-1
                manhattan_distance+=score

        return manhattan_distance
    else:
        for i in range(n):
            if my_array[i]==-1:
                continue
            for j in range(i+1,n):
                if my_array[j]==-1:
                    continue
                score = abs(my_array[i]-my_array[j]) + abs(i-j)
                if score < 3:
                    score = n - 1
                manhattan_distance += score

        return manhattan_distance




def gen_succesors(board,n,closed,open_keys,open,type,H1,depth):
    if type=="board_array":
        for i in range(n):
            for j in range(n):
                new_board = [row[:] for row in board]
                if board[i][j] == 0:
                    new_board[i][j] = 1
                    key = tuple(tuple(row) for row in new_board)
                    if key not in closed and key not in open_keys:
                        priority = H1(new_board)
                        heapq.heappush(open, (priority, new_board,depth+1))
                        open_keys.add(key)
    else:
        for i in range(n):
            if board[i] == -1:
                for j in range(n):
                    new_board = board[:]
                    if j not in board:
                        new_board[i] = j
                        key = tuple(new_board)
                        if key not in closed and key not in open_keys:
                            priority = H1(new_board)
                            heapq.heappush(open, (priority, new_board, depth+1))
                            open_keys.add(key)

def BestFS(n,name,H1):
    closed = set()
    open_keys = set()
    if name=="board_array":
        my_array = [[0] * n for _ in range(n)]
        key_open = tuple(tuple(row) for row in my_array)
        open_keys.add(key_open)
        open=[(0,my_array,0)]
        while open:
            priority, my_array, depth=heapq.heappop(open)
            key = tuple(tuple(row) for row in my_array)
            open_keys.remove(key)
            no_conflict=conflicts(my_array)
            if no_conflict:
                key_closed = tuple(tuple(row) for row in my_array)
                closed.add(key_closed)
                return my_array,open_keys,closed
            elif depth <n:
                gen_succesors(my_array,n,closed,open_keys,open,"board_array",H1,depth)
                key_closed = tuple(tuple(row) for row in my_array)
                closed.add(key_closed)
    else:
        my_array = [-1] * n
        closed = set()
        key_open = tuple(my_array)
        open_keys.add(key_open)
        open = [(0,my_array,0)]
        while open:
            priority, my_array,depth = heapq.heappop(open)
            key = tuple(my_array)
            open_keys.remove(key)
            no_conflict=conflicts(my_array)
            if no_conflict:
                key_closed = tuple(my_array)
                closed.add(key_closed)
                return my_array, open_keys, closed
            elif depth <n:
                gen_succesors(my_array, n, closed,open_keys,open, "board_perm",H1,depth)
                key_closed = tuple(my_array)
                closed.add(key_closed)




if __name__ == "__main__":
     from tests.test_bfs_dfs import experiment
     from tests.analyse_plots import plot_metric

     experiment()
     df = pd.read_csv('result6.csv',sep=';')
     plot_metric(df, "time", 'Czas wykonania dla poszczególnych N')
     plot_metric(df, "open_count", "Zbiór open dla poszczególnych N")
     plot_metric(df, "closed_count", "Zbiór closed dla poszczególnych N")

