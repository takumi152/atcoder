import copy

def main():
    h, w, k = map(int, input().split())
    c = [None for _ in range(h)]
    for i in range(h):
        c[i] = list(input())

    ans = 0
    for i in range(1 << h):
        c1 = copy.deepcopy(c)
        x = i
        p = 0
        while x > 0:
            if x & 1 == 1:
                c1[p] = ['x'] * w
            x = x >> 1
            p += 1
        for j in range(1 << w):
            c2 = copy.deepcopy(c1)
            x = j
            p = 0
            while x > 0:
                if x & 1 == 1:
                    for l in range(h):
                        c2[l][p] = 'x'
                x = x >> 1
                p += 1
            count = 0
            for l in range(h):
                for m in range(w):
                    if c2[l][m] == '#':
                        count += 1
            if count == k:
                ans += 1

    print(ans)

if __name__ == '__main__':
    main()
