def main():
    a, b = map(int, input().split())

    ans = 1
    for i in range(1, b+1):
        p = i
        c = 0
        while p <= b:
            if a <= p:
                c += 1
            p += i
        if c >= 2:
            ans = i

    print(ans)

if __name__ == '__main__':
    main()
