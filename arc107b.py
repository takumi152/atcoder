def main():
    n, k = map(int, input().split())

    comb = [0 for _ in range(2*n+1)]
    for i in range(2, n+2):
        comb[i] = i-1
        comb[-i+1] = i-1

    pos = 2
    neg = 2
    if k > 0:
        pos += k
    else:
        neg -= k

    ans = 0
    while pos < 2*n+1 and neg < 2*n+1:
        ans += comb[pos] * comb[neg]
        pos += 1
        neg += 1

    print(ans)

if __name__ == '__main__':
    main()
