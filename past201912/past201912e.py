def main():
    buf = input()
    buflist = buf.split()
    n = int(buflist[0])
    q = int(buflist[1])
    s = []
    for i in range(q):
        buf = input()
        buflist = buf.split()
        s.append(list(map(int, buflist)))

    for i in range(q):
        for j in range(len(s[i])):
            s[i][j] -= 1

    follow = []
    for i in range(n):
        follow.append([])
        for j in range(n):
            follow[i].append(False)
    for i in range(q):
        if s[i][0] == 0:
            follow[s[i][1]][s[i][2]] = True
        elif s[i][0] == 1:
            for j in range(n):
                if s[i][1] == j:
                    continue
                if follow[j][s[i][1]]:
                    follow[s[i][1]][j] = True
        elif s[i][0] == 2:
            new_follow = set()
            for j in range(n):
                if follow[s[i][1]][j]:
                    for k in range(n):
                        if follow[j][k]:
                            new_follow.add(k)
            for x in new_follow:
                if x != s[i][1]:
                    follow[s[i][1]][x] = True

    for i in range(n):
        for j in range(n):
            if follow[i][j]:
                follow[i][j] = 'Y'
            else:
                follow[i][j] = 'N'

    for i in range(n):
        print(''.join(follow[i]))

if __name__ == '__main__':
    main()
