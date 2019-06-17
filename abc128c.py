
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    K = []
    S = []
    for i in range(M):
        buf = input()
        buflist = buf.split()
        K.append(int(buflist[0]))
        S.append(list(map(int, buflist[1:])))
    buf = input()
    buflist = buf.split()
    P = list(map(int, buflist))
    on_pattern = 0
    for i in range(2 ** N):
        sw = []
        for j in range(N):
            sw.append(i >> j & 1 > 0)
        all_on = True
        for j in range(M):
            on_count = 0
            for x in S[j]:
                if sw[x-1]:
                    on_count += 1
            if on_count % 2 != P[j]:
                all_on = False
                break
        if all_on:
            on_pattern += 1
    print(on_pattern)


if __name__ == '__main__':
    main()
