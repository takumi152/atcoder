def main():
    n, x = map(int, input().split())
    a = list(map(int, input().split()))

    ans = []
    for i in range(n):
        if a[i] != x:
            ans.append(a[i])

    print(' '.join([str(x) for x in ans]))

if __name__ == '__main__':
    main()
