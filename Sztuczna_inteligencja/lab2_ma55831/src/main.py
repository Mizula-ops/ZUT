
import numpy as np

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

def gen_succesors(board,n,closed,open,type,depth):
    if type=="board_array":
        for i in range(n):
            for j in range(n):
                new_board = [row[:] for row in board]
                if board[i][j] == 0:
                    new_board[i][j] = 1
                    key = tuple(tuple(row) for row in new_board)
                    if (key, depth + 1) not in closed and (new_board, depth +1) not in open:
                        open.append((new_board, depth + 1))
        return open
    else:
        for i in range(n):
            for j in range(n):
                new_board = board[:]
                if board[i] == -1 and j not in board:
                    new_board[i] = j
                    key = tuple(new_board)
                    if (key, depth + 1) not in closed and (new_board, depth +1) not in open:
                        open.append((new_board, depth + 1))
        return open

def bfs(n,name):
    if name=="board_array":
        my_array = [[0] * n for _ in range(n)]
        closed= set()
        open=[(my_array,0)]
        while open:
            my_array, depth=open.pop(0)
            no_conflict=conflicts(my_array)
            if no_conflict == True:
                key_closed = tuple(tuple(row) for row in my_array)
                closed.add((key_closed,depth))
                return my_array,open,closed

            elif no_conflict == False and depth<n:
                open = gen_succesors(my_array,n,closed,open,"board_array",depth)
                key_closed = tuple(tuple(row) for row in my_array)
                closed.add((key_closed, depth))
    elif name == "board_perm":
        my_array = [-1] * n
        closed = set()
        open = [(my_array,0)]
        while open:
            my_array,depth=open.pop(0)
            no_conflict=conflicts(my_array)
            if no_conflict == True:
                key_closed = tuple(my_array)
                closed.add(key_closed)
                return my_array, open, closed
            elif no_conflict ==False and depth<n:
                open = gen_succesors(my_array, n, closed, open, "board_perm", depth)
                key_closed = tuple(my_array)
                closed.add((key_closed,depth))

def dfs(n,name):
    if name=="board_array":
        my_array = [[0] * n for _ in range(n)]
        closed= set()
        open=[(my_array,0)]
        while open:
            my_array, depth=open.pop()
            no_conflict=conflicts(my_array)
            if no_conflict == True:
                key_closed = tuple(tuple(row) for row in my_array)
                closed.add((key_closed,depth))
                return my_array, open, closed
            elif no_conflict ==False and depth<n:
                open = gen_succesors(my_array, n, closed, open, "board_array", depth)
                key_closed = tuple(tuple(row) for row in my_array)
                closed.add((key_closed, depth))

    elif name == "board_perm":
        my_array = [-1] * n
        closed = set()
        open = [(my_array,0)]
        while open:
            my_array,depth=open.pop()
            no_conflict=conflicts(my_array)
            if no_conflict == True:
                key_closed = tuple(my_array)
                closed.add(key_closed)
                return my_array, open, closed
            elif no_conflict ==False and depth<n:
                open = gen_succesors(my_array, n, closed, open, "board_perm", depth)
                key_closed = tuple(my_array)
                closed.add((key_closed,depth))
solution,open,closed =dfs(9,"board_perm")
print(solution)

# if __name__ == "__main__":
#     from tests.test_bfs_dfs import experiment
#     print("start")
#     experiment()
#     print("end")
