def main():
    n = int(input())

    k = 0
    d = []
    x = n
    while x > 0:
        d.append(x % 10)
        k += 1
        x //= 10

    ans = 10000
    for i in range(2 ** k - 1):
        score = 0
        total = 0
        for j in range(k):
            if i & (1 << j):
                score += 1
            else:
                total += d[j]

        if total % 3 == 0 and score < ans:
            ans = score

    if ans == 10000:
        print(-1)
    else:
        print(ans)

if __name__ == '__main__':
    main()
