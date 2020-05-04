def main():
    x = int(input())

    ans = ((x + 10) // 11) * 2 - 1
    if (x + 10) % 11 > 5:
        ans += 1

    print(ans)

if __name__ == '__main__':
    main()
