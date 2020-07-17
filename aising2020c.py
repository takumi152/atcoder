def main():
    n = int(input())

    ans = [0 for _ in range(n+1)]
    x = 1
    y = 1
    z = 1
    while True:
        f = x * x + y * y + z * z + x * y + y * z + z * x
        if f > n:
            if z > 1:
                y += 1
                z = 1
            elif y > 1:
                x += 1
                y = 1
            else:
                break
        else:
            ans[f] += 1
            z += 1

    for i in range(1, n+1):
        print(ans[i])

if __name__ == '__main__':
    main()
