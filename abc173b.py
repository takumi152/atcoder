def main():
    n = int(input())
    s = ['' for _ in range(n)]
    for i in range(n):
        s[i] = input()

    ac = 0
    wa = 0
    tle = 0
    re = 0
    for i in range(n):
        if s[i] == 'AC':
            ac += 1
        elif s[i] == 'WA':
            wa += 1
        elif s[i] == 'TLE':
            tle += 1
        elif s[i] == 'RE':
            re += 1

    print(f'AC x {ac}')
    print(f'WA x {wa}')
    print(f'TLE x {tle}')
    print(f'RE x {re}')


if __name__ == '__main__':
    main()
