def main():
    n = int(input())
    a = list(map(int, input().split()))

    mask = 0
    for i in range(n):
        mask = mask ^ a[i]

    ans = [0 for _ in range(n)]
    for i in range(n):
        ans[i] = a[i] ^ mask

    print(' '.join([str(x) for x in ans]))

if __name__ == '__main__':
    main()
