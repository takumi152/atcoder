def main():
    n, k, s = map(int, input().split())

    ans = [(10 ** 9 if s != 10 ** 9 else 10 ** 9 - 1) for _ in range(n)]
    for i in range(k):
        ans[i] = s

    print(' '.join([str(x) for x in ans]))

if __name__ == '__main__':
    main()
