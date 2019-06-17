
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    LR = []
    for i in range(M):
        buf = input()
        buflist = buf.split()
        LR.append((int(buflist[0]), int(buflist[1])))
    LR = list(sorted(LR))
    left_limit = LR[-1][0]
    LR = list(sorted(LR, key=lambda x:x[1]))
    right_limit = LR[0][1]
    print(max(0, right_limit - left_limit + 1))

if __name__ == '__main__':
    main()
