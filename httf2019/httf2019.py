import time
import random
import copy

VISUALIZE = True

def main():
    t_start = time.time()
    T_LIMIT = 3.0
    buf = input()
    buflist = buf.split()
    N = int(buflist[0]) # = 500
    M = int(buflist[1]) # = 29
    L = int(buflist[2]) # = 300
    S = []
    for i in range(N):
        buf = input()
        S.append(buf)
    # board initialization
    board = []
    for i in range(M):
        board.append(list("." * M))
    for i in range(M):
        for j in range(M):
            if i == 0 or i == M - 1 or j == 0 or j == M - 1:
                board[i][j] = "#"
    # try to find best board with random panel placement
    PANEL_LIST = ["#", "D", "T", "L", "R", "."]
    WALL_CHANCE = 0.000
    DOUBLE_CHANCE = 0.00 + WALL_CHANCE
    TRIPLE_CHANCE = 0.00 + DOUBLE_CHANCE
    LEFT_CHANCE = 0.00 + TRIPLE_CHANCE
    RIGHT_CHANCE = 0.00 + LEFT_CHANCE
    NOTHING_CHANCE = 1.0
    PROB_LIST = [WALL_CHANCE, DOUBLE_CHANCE, TRIPLE_CHANCE, LEFT_CHANCE, RIGHT_CHANCE, NOTHING_CHANCE]
    best_board = copy.deepcopy(board)
    best_score = simulate_board(board, M, S)
    while True:
        new_board = copy.deepcopy(board)
        # randomly place panel
        for i in range(1, M-1):
            for j in range(1, M-1):
                roll = random.random()
                for k, prob in enumerate(PROB_LIST):
                    if roll < prob:
                        new_board[i][j] = PANEL_LIST[k]
                        break
        for i in range(M):
            for j in range(M):
                if (i == 1 or i == M - 2 or j == 1 or j == M - 2) and not (i == 0 or i == M - 1 or j == 0 or j == M - 1):
                    roll = random.random()
                    if roll < 1.0:
                        new_board[i][j] = "R"
        # wall is not allowed on center
        if new_board[(M - 1) // 2][(M - 1) // 2] == "#":
            new_board[(M - 1) // 2][(M - 1) // 2] = "." # remove wall
        # simulate board
        score = simulate_board(new_board, M, S)
        # is score higher than the best one?
        if score > best_score:
            best_score = score
            best_board = copy.deepcopy(new_board)
        # score and time visualization
        if VISUALIZE:
            print(score, T_LIMIT - abs(time.time() - t_start), flush=True)
        # finish if time left < 0.2 seconds
        if T_LIMIT - abs(time.time() - t_start) < 0.2:
            break
    # print final board
    for i in range(M):
        print("".join(best_board[i]))
    if VISUALIZE:
        print(best_score)

def simulate_board(board, M, S):
    stop_count = []
    for i in range(M):
        stop_count.append([0] * M)
    for test_case in S:
        # initialize robot position
        x = (M - 1) // 2
        y = (M - 1) // 2
        direction = 0
        for command in test_case:
            # check for Double / Triple panel
            executions = 1
            if board[y][x] == "D":
                executions = 2
            elif board[y][x] == "T":
                executions = 3
            for execution in range(executions):
                # move forward
                if command == "S":
                    if direction == 0: # up
                        if not board[y-1][x] == "#": # check for a wall
                            y -= 1
                    elif direction == 1: # right
                        if not board[y][x+1] == "#": # check for a wall
                            x += 1
                    elif direction == 2: # down
                        if not board[y+1][x] == "#": # check for a wall
                            y += 1
                    elif direction == 3: # left
                        if not board[y][x-1] == "#": # check for a wall
                            x -= 1
                # turn left
                elif command == "L":
                    if board[y][x] == "R": # check for Right panel
                        direction = (direction + 1) % 4 # turn right instead
                    else:
                        direction = (direction - 1) % 4
                elif command == "R":
                    if board[y][x] == "L": # check for Left panel
                        direction = (direction - 1) % 4 # turn left instead
                    else:
                        direction = (direction + 1) % 4
        stop_count[y][x] += 1 # add stop count of final position
    # calculate final score
    score = 0
    for i in range(M):
        for j in range(M):
            if stop_count[i][j] == 1:
                score += 10
            elif stop_count[i][j] == 2:
                score += 3
            elif stop_count[i][j] == 3:
                score += 1
    return score

if __name__ == '__main__':
    main()
