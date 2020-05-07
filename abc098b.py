def main():
    n = int(input())
    s = input()

    ans = 0
    for i in range(n):
        x = set()
        for j in range(i):
            x.add(s[j])
        y = set()
        for j in range(i, n):
            if s[j] in x:
                y.add(s[j])
        ans = max(ans, len(y))

    print(ans)

if __name__ == '__main__':
    main()
