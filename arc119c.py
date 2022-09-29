def main():
    n = int(input())
    a = list(map(int, input().split()))

    c = {0: 1}
    p = 0
    for i in range(n):
        p = a[i] - p
        pa = p if i % 2 == 0 else -p
        if pa not in c:
            c[pa] = 1
        else:
            c[pa] += 1

    ans = 0
    for k, v in c.items():
        ans += v * (v - 1) // 2

    print(ans)

if __name__ == '__main__':
    main()
