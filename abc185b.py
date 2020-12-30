def main():
    n, m, t = map(int, input().split())
    ab = [0 for _ in range(m)]
    for i in range(m):
        ab[i] = list(map(int, input().split()))

    battery = n
    now = 0
    for i in range(m):
        battery -= ab[i][0] - now
        now = ab[i][0]
        if battery <= 0:
            print('No')
            return
        battery += ab[i][1] - now
        if battery > n:
            battery = n
        now = ab[i][1]

    battery -= t - now
    if battery <= 0:
        print('No')
    else:
        print('Yes')

if __name__ == '__main__':
    main()
