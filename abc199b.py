def main():
    n = int(input())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))

    mask = [True for _ in range(1001)]
    for i in range(n):
        for j in range(a[i]):
            mask[j] = False
        for j in range(b[i]+1, 1001):
            mask[j] = False

    ans = 0
    for x in mask:
        if x:
            ans += 1

    print(ans)

if __name__ == '__main__':
    main()
