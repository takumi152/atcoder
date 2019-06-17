
def main():
    buf = input()
    N = int(buf)
    D = []
    for i in range(N):
        buf = input()
        d = int(buf)
        D.append(d)
    D = list(set(sorted(D)))
    size = len(D)
    print(size)

if __name__ == '__main__':
    main()
