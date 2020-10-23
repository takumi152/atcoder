def main():
    n = int(input())

    p = 1
    ans = []
    while p * p <= n:
        if n % p == 0:
            ans.append(p)
            if p * p < n:
                ans.append(n // p)
        p += 1

    ans.sort()
    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
