def main():
    n = int(input())
    a = ['' for _ in range(n)]
    for i in range(n):
        a[i] = input()

    ans = ''
    for i in range((n+1)//2):
        c_top = set()
        c_bottom = set()
        for j in range(n):
            c_top.add(a[i][j])
            c_bottom.add(a[n-i-1][j])
        found = False
        for x in c_top:
            if x in c_bottom:
                found = True
                ans += x
                break
        if not found:
            print(-1)
            return

    if n % 2 == 1:
        ans += ans[::-1][1:]
    else:
        ans += ans[::-1]

    print(ans)

if __name__ == '__main__':
    main()
