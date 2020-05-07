def main():
    n = int(input())
    s = ['' for _ in range(n)]
    for i in range(n):
        s[i] = input()

    appearance = dict()
    for i in range(n):
        s[i] = tuple(sorted(s[i]))
        if s[i] not in appearance:
            appearance.update({s[i]: 1})
        else:
            appearance[s[i]] += 1

    ans = 0
    for _, x in appearance.items():
        ans += (x * (x - 1)) // 2

    print(ans)

if __name__ == '__main__':
    main()
