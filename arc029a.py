import itertools

def main():
    n = int(input())
    t = []
    for i in range(n):
        t.append(int(input()))

    ans = 1000000007
    for pt in itertools.permutations(t):
        for m in range(1 << n):
            a = [0, 0]
            for i in range(n):
                a[(m>>i)&1] += pt[i]
            if max(a) < ans:
                ans = max(a)

    print(ans)

if __name__ == '__main__':
    main()
