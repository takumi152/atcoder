import math

def main():
    n, q = map(int, input().split())
    a = list(map(int, input().split()))
    s = list(map(int, input().split()))

    gcdPartial = [a[0]]
    for i in range(1, n):
        gcdPartial.append(math.gcd(gcdPartial[-1], a[i]))

    ans = []
    for i in range(q):
        ng = 0
        ok = n
        while ng < ok:
            center = (ng + ok) // 2
            if math.gcd(gcdPartial[center], s[i]) == 1:
                ok = center
            else:
                ng = center + 1
        if ng == n:
            ans.append(math.gcd(gcdPartial[-1], s[i]))
        else:
            ans.append(ok + 1)

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
