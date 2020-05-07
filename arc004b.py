def main():
    n = int(input())
    d = [None for _ in range(n)]
    for i in range(n):
        d[i] = int(input())

    d.sort()

    ans_max = sum(d)
    ans_min = max(0, d[-1] * 2 - sum(d))

    print(ans_max)
    print(ans_min)

if __name__ == '__main__':
    main()
