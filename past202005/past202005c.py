def main():
    a, r, n = map(int, input().split())

    if r == 1:
        print(a)
    else:
        ans = a
        for i in range(n-1):
            ans *= r
            if ans > 10 ** 9:
                break
        if ans > 10 ** 9:
            print('large')
        else:
            print(ans)

if __name__ == '__main__':
    main()
