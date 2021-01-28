def main():
    n, c = map(int, input().split())
    abc = [None for _ in range(n)]
    for i in range(n):
        abc[i] = tuple(map(int, input().split()))

    imos = []
    for x in abc:
        imos.append((x[0], x[2]))
        imos.append((x[1]+1, -x[2]))

    imos.sort()

    ans = 0
    price = 0
    for i in range(len(imos)-1):
        price += imos[i][1]
        ans += min(price, c) * (imos[i+1][0] - imos[i][0])

    print(ans)

if __name__ == '__main__':
    main()
