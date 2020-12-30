def main():
    a, b = map(int, input().split())
    c, d = map(int, input().split())

    ans = a * d - b * c

    print(ans)

if __name__ == '__main__':
    main()
