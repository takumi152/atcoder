def main():
    n = int(input())
    a = list(map(int, input().split()))

    target = [0]
    prev = a[0]
    for i in range(1, n):
        if a[i] > prev:
            target.append(i)
            prev = a[i]

    ax = [None] * n
    for i in range(n):
        ax[i] = [a[i], i]
    ax.sort(reverse=True)

    ans = [0] * n
    mult = 1
    for i in range(n-1):
        ans[target[-1]] += (ax[i][0] - ax[i+1][0]) * mult
        if (ax[i+1][0] < a[target[-1]] and ax[i+1][1] < target[-1]):
            target.pop()
        mult += 1
    ans[target[-1]] += ax[n-1][0] * mult

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
