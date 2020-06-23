def main():
    n = int(input())
    a = list(map(int, input().split()))
    q = int(input())
    bc = [None for _ in range(q)]
    for i in range(q):
        bc[i] = list(map(int, input().split()))

    count = [0 for i in range(100001)]
    for i in range(n):
        count[a[i]] += 1

    ans = [0 for _ in range(q+1)]
    for i in range(100001):
        ans[0] += i * count[i]
    for i in range(q):
        ans[i+1] = ans[i] - count[bc[i][0]] * bc[i][0] + count[bc[i][0]] * bc[i][1]
        count[bc[i][1]] += count[bc[i][0]]
        count[bc[i][0]] = 0

    for i in range(1, q+1):
        print(ans[i])


if __name__ == '__main__':
    main()
