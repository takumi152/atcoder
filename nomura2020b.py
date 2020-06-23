def main():
    t = list(input())

    n = len(t)
    p = 0
    while p < n:
        if p == n-1:
            if t[p] == '?':
                t[p] = 'D'
            p += 1
        else:
            if t[p] == 'P' and t[p+1] == '?':
                t[p+1] = 'D'
                p += 2
            elif t[p] == '?' and t[p+1] == 'D':
                t[p] = 'P'
                p += 2
            elif t[p] == '?' and t[p+1] == '?':
                t[p] = 'P'
                t[p+1] = 'D'
                p += 2
            elif t[p] == '?':
                t[p] = 'D'
                p += 1
            else:
                p += 1

    print(''.join(t))

if __name__ == '__main__':
    main()
