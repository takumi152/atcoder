def main():
    n = int(input())
    h = [None] * n
    for i in range(n):
        h[i] = int(input())

    ascending = True
    now = 1
    best = 1
    for i in range(n-1):
        if h[i] < h[i+1]:
            if ascending:
                now += 1
                best = max(best, now)
            else:
                ascending = True
                now = 2
                best = max(best, now)
        else:
            if not ascending:
                now += 1
                best = max(best, now)
            else:
                ascending = False
                now += 1
                best = max(best, now)

    print(best)

if __name__ == '__main__':
    main()
