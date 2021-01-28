def main():
    n, k = map(int, input().split())
    a = list(map(int, input().split()))

    a.sort()

    mex = [0 for _ in range(k)]

    p = -1
    last = -1
    for i in range(n):
        if a[i] == last:
            p += 1
            if p < k:
                if mex[p] == a[i]:
                    mex[p] += 1
        else:
            last = a[i]
            p = 0
            if mex[p] == a[i]:
                mex[p] += 1

    print(sum(mex))

if __name__ == '__main__':
    main()
