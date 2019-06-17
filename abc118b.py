
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    K = []
    A = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        K.append(int(buflist[0]))
        A.append(list(map(int, buflist[1:])))
    liked = []
    for i in range(M):
        liked.append(0)
    for i in range(N):
        for j in range(K[i]):
            liked[A[i][j]-1] += 1
    liked_count = 0
    for i in range(M):
        if liked[i] == N:
            liked_count += 1
    print(liked_count)

if __name__ == '__main__':
    main()
