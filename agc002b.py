def main():
    n, m = map(int, input().split())
    x = [None] * m
    y = [None] * m
    for i in range(m):
        x[i], y[i] = map(int, input().split())

    ball = [1] * n
    red = [False] * n
    red[0] = True
    for i in range(m):
        if red[x[i]-1]:
            red[y[i]-1] = True
        ball[x[i]-1] -= 1
        ball[y[i]-1] += 1
        if ball[x[i]-1] == 0:
            red[x[i]-1] = False

    print(sum(red))

if __name__ == '__main__':
    main()
