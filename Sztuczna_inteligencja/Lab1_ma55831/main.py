import numpy as np
import random
import time
import csv
def generate_board1(n,struct):
    if(struct=="board_tuple"):
        board_tuple=[(random.randint(0, n - 1), random.randint(0, n - 1)) for _ in range(n)]
        return board_tuple
    elif(struct=="board_array"):
        board_array = [[0] * n for _ in range(n)]
        board_help = [random.randint(0, n - 1) for _ in range(n)]
        for i in range(n):
            board_array[board_help[i]][i] = 1
        return board_array
    elif(struct=="board_vector"):
        board_vector=[random.randint(0, n - 1) for _ in range(n)]
        return(board_vector)
    elif(struct=="board_perm"):
        board_perm=list(range(n))
        random.shuffle(board_perm)
        return(board_perm)

def conflicts(board):
    my_array=np.array(board)
    n=my_array.shape[0]
    help_array = [-1] * n
    if my_array.ndim == 2 and my_array.shape[1]==2:
        for r, c in my_array:
            if help_array[r] != -1 or c in help_array:
                return False
            help_array[r] = c
    elif my_array.ndim == 2:
        for r in range(n):
            for c in range(n):
                if my_array[r][c] == 1:
                    if help_array[r] !=-1 or  c in help_array:
                        return False
                    help_array[r]=c
    elif(len(my_array) !=len(set(my_array))):
        return False
    if -1 in help_array:
        help_array=my_array
    for i in range(n):
        for j in range(i+1,n):
            if abs(help_array[i]-help_array[j]) == abs(i-j):
                return False
    return True

def measure_time(N,struct):
    iteration=100
    is_True=0
    total_time=0
    for i in range(iteration):
        start=time.perf_counter()
        board=generate_board1(N,struct)
        is_conflict= conflicts(board)
        stop=time.perf_counter()
        elapsed = stop - start
        total_time += elapsed
        if is_conflict:
            is_True+=1
    return iteration-(is_True/iteration), total_time/iteration

def experiment():
    Ns=[4,8,16,32,64,128]
    representations=["board_tuple","board_array","board_vector","board_perm"]
    with open('result2.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["N","representation_type","execution_time","conflict_rate"])
        for struct in representations:
            for N in Ns:
                avg_hit, avg_time= measure_time(N,struct)
                writer.writerow([N, struct, format(avg_time,".10f"), avg_hit])

board_tuple = [(0, 2), (1, 0), (2, 3), (3, 1)]
board_array1 = [[0, 0, 1, 0], [1, 0, 0, 0], [0, 0, 0, 1], [0, 1, 0, 0]]
board_vector = [2, 0, 3, 1]
board_perm = (2, 0, 3, 1)

tab_struct=[board_tuple, board_array1, board_vector, board_perm]
for struct in tab_struct:
    print(conflicts(struct))
experiment()



