
MOD = int(1e9) + 7

def main():
    buf = input()
    N = int(buf)
    S = []
    for i in range(2):
        buf = input()
        S.append(buf)
    last_pattern = None # 1: 1 x 2, 2: 2 x 1
    combination = 1
    point = 0
    while point < N:
        if S[0][point] == S[1][point]: # 1 x 2
            if last_pattern is None: # first column
                combination *= 3
            elif last_pattern == 1:
                combination *= 2 # 2 possible colors
            else: # last_pattern == 2
                combination *= 1 # 1 possible color
            last_pattern = 1
            point += 1
        else: # 2 x 1
            if last_pattern is None: # first column
                combination *= 6
            elif last_pattern == 1:
                combination *= 2 # 2 possible color combinations
            else: # last_pattern == 2
                combination *= 3 # 3 possible color combinations
            last_pattern = 2
            point += 2
        combination %= MOD
    print(combination)

if __name__ == '__main__':
    main()
