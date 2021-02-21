def main():
    x = int(input())

    ans = 100 - x % 100
    if ans == 0:
        ans = 100

    print(ans)

if __name__ == '__main__':
    main()
