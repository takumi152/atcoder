def main():
    n = int(input())
    a = list(map(int, input().split()))

    best = 0
    ans = 0
    for i in range(2, 1001):
        score = 0
        for j in range(n):
            if a[j] % i == 0:
                score += 1
        if score > best:
            best = score
            ans = i

    print(ans)

if __name__ == '__main__':
    main()
