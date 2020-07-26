def main():
    n, k = map(int, input().split())
    a = list(map(int, input().split()))

    ans = []
    for i in range(k, n):
        if a[i-k] < a[i]:
            ans.append('Yes')
        else:
            ans.append('No')

    for x in ans:
        print(x)


if __name__ == '__main__':
    main()
