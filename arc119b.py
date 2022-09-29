def main():
    n = int(input())
    s = input()
    t = input()

    s0 = 0
    t0 = 0
    for i in range(n):
        if s[i] == '0':
            s0 += 1
        if t[i] == '0':
            t0 += 1
    if s0 != t0:
        print(-1)
        return

    sp = 0
    tp = 0
    ans = 0
    while sp < n and tp < n:
        while sp < n:
            if s[sp] != '0':
                sp += 1
            else:
                break
        while tp < n:
            if t[tp] != '0':
                tp += 1
            else:
                break
        if sp != tp:
            ans += 1
        sp += 1
        tp += 1

    print(ans)

if __name__ == '__main__':
    main()
