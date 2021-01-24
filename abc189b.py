def main():
    n, x = map(int, input().split())
    vp = [None for _ in range(n)]
    for i in range(n):
        vp[i] = tuple(map(int, input().split()))

    alcohol = 0
    for i in range(n):
        alcohol += vp[i][0] * vp[i][1]
        if alcohol > x * 100:
            print(i+1)
            return

    print(-1)

if __name__ == '__main__':
    main()
