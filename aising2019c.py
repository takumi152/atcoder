
TTYPE = 0
X = 1
Y = 2

def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    S = []
    for i in range(H):
        buf = input()
        S.append(buf)
    searched = []
    for i in range(H):
        searched.append([])
        for j in range(W):
            searched[i].append(False)
    group_white = []
    group_black = []
    for i in range(H):
        for j in range(W):
            if searched[i][j]:
                continue
            else:
                search_stack = [[S[i][j], i, j]]
                group_white.append(0)
                group_black.append(0)
                while len(search_stack) > 0:
                    target = search_stack.pop()
                    if searched[target[X]][target[Y]]:
                        continue
                    if target[TTYPE] == '.':
                        group_white[-1] += 1
                    else:
                        group_black[-1] += 1
                    searched[target[X]][target[Y]] = True
                    if target[X] > 0:
                        if not searched[target[X]-1][target[Y]] and ((target[TTYPE] == '.' and S[target[X]-1][target[Y]] == "#") or (target[TTYPE] == '#' and S[target[X]-1][target[Y]] == ".")):
                            search_stack.append([S[target[X]-1][target[Y]], target[X]-1, target[Y]])
                    if target[Y] > 0:
                        if not searched[target[X]][target[Y]-1] and ((target[TTYPE] == '.' and S[target[X]][target[Y]-1] == "#") or (target[TTYPE] == '#' and S[target[X]][target[Y]-1] == ".")):
                            search_stack.append([S[target[X]][target[Y]-1], target[X], target[Y]-1])
                    if target[X] < H-1:
                        if not searched[target[X]+1][target[Y]] and ((target[TTYPE] == '.' and S[target[X]+1][target[Y]] == "#") or (target[TTYPE] == '#' and S[target[X]+1][target[Y]] == ".")):
                            search_stack.append([S[target[X]+1][target[Y]], target[X]+1, target[Y]])
                    if target[Y] < W-1:
                        if not searched[target[X]][target[Y]+1] and ((target[TTYPE] == '.' and S[target[X]][target[Y]+1] == "#") or (target[TTYPE] == '#' and S[target[X]][target[Y]+1] == ".")):
                            search_stack.append([S[target[X]][target[Y]+1], target[X], target[Y]+1])
    total = 0
    for i in range(len(group_black)):
        total += group_black[i] * group_white[i]
    print(total)

if __name__ == '__main__':
    main()
