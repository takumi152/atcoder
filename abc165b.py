def main():
    x = int(input())

    a = 100
    ans = 0
    while a < x:
        a = (a * 101) // 100;
        ans += 1

    print(ans)

if __name__ == '__main__':
    main()
