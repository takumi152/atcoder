import bisect

def main():
    a, b, q = map(int, input().split())
    s = [0 for _ in range(a)]
    for i in range(a):
        s[i] = int(input())
    t = [0 for _ in range(b)]
    for i in range(b):
        t[i] = int(input())
    x = [0 for _ in range(q)]
    for i in range(q):
        x[i] = int(input())

    ans = [10**18 for _ in range(q)]
    for i in range(q):
        next1 = bisect.bisect(s, x[i])
        if next1 > 0:
            d1 = x[i] - s[next1-1]
            next2 = bisect.bisect(t, s[next1-1])
            if next2 > 0:
                d2 = d1 + (s[next1-1] - t[next2-1])
                if d2 < ans[i]:
                    ans[i] = d2
            if next2 < b:
                d2 = d1 + (t[next2] - s[next1-1])
                if d2 < ans[i]:
                    ans[i] = d2
        if next1 < a:
            d1 = s[next1] - x[i]
            next2 = bisect.bisect(t, s[next1])
            if next2 > 0:
                d2 = d1 + (s[next1] - t[next2-1])
                if d2 < ans[i]:
                    ans[i] = d2
            if next2 < b:
                d2 = d1 + (t[next2] - s[next1])
                if d2 < ans[i]:
                    ans[i] = d2

        next1 = bisect.bisect(t, x[i])
        if next1 > 0:
            d1 = x[i] - t[next1-1]
            next2 = bisect.bisect(s, t[next1-1])
            if next2 > 0:
                d2 = d1 + (t[next1-1] - s[next2-1])
                if d2 < ans[i]:
                    ans[i] = d2
            if next2 < a:
                d2 = d1 + (s[next2] - t[next1-1])
                if d2 < ans[i]:
                    ans[i] = d2
        if next1 < b:
            d1 = t[next1] - x[i]
            next2 = bisect.bisect(s, t[next1])
            if next2 > 0:
                d2 = d1 + (t[next1] - s[next2-1])
                if d2 < ans[i]:
                    ans[i] = d2
            if next2 < a:
                d2 = d1 + (s[next2] - t[next1])
                if d2 < ans[i]:
                    ans[i] = d2

    for i in range(q):
        print(ans[i])

if __name__ == '__main__':
    main()
