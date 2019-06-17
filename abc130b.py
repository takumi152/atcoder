
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    X = int(buflist[1])
    buf = input()
    buflist = buf.split()
    L = list(map(int, buflist))
    bound = [0]
    for i in range(N):
        bound.append(bound[-1] + L[i])
    for i, x in enumerate(bound):
        if x > X:
            print(i)
            return
    print(N+1)

if __name__ == '__main__':
    main()
