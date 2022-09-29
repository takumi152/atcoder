def main():
    n, q = map(int, input().split())
    a = list(map(int, input().split()))
    k = [None for _ in range(q)]
    for i in range(q):
        k[i] = (int(input()), i)

    k.sort(key = lambda x: x[0])

    ans = []
    p = 0
    for i in range(q):
        while p < n:
            if k[i][0] + p < a[p]:
                break
            p += 1
        ans.append((k[i][0] + p, k[i][1]))

    ans.sort(key = lambda x: x[1])

    for x, _ in ans:
        print(x)

if __name__ == '__main__':
    main()
