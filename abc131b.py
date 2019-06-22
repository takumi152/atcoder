
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    L = int(buflist[1])
    apple = []
    for i in range(N):
        apple.append(L+i)
    nearest = int(1e9)+7
    for i in range(N):
        if abs(apple[i]) < abs(nearest):
            nearest = apple[i]
    print(sum(apple) - nearest)

if __name__ == '__main__':
    main()
