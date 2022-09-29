def main():
    n = int(input())

    ans = n
    for b in range(64):
        if 2 ** b > n:
            break
        a = n // (2 ** b)
        c = n % (2 ** b)
        if a + b + c < ans:
            ans = a + b + c

    print(ans)

if __name__ == '__main__':
    main()
