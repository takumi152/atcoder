def main():
    n = int(input())
    a = list(map(int, input().split()))

    res = 0
    for x in a:
        res ^= x

    for x in a:
        if x == res:
            print('Win')
            return

    if n % 2 == 0:
        print('Lose')
    else:
        print('Win')

if __name__ == '__main__':
    main()
