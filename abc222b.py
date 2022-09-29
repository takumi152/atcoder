def main():
    n, p = map(int, input().split())
    a = list(map(int, input().split()))

    print(sum([(1 if x < p else 0) for x in a]))

if __name__ == '__main__':
    main()
