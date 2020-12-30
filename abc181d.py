def main():
    s = input()

    lim = 1000

    if len(s) == 1:
        lim = 10
        s = '00' + s
    elif len(s) == 2:
        lim = 100
        s = '0' + s

    num_count = dict()
    for i in range(10):
        num_count[f'{i}'] = 0
    for x in s:
        num_count[x] += 1

    for i in range(0, lim, 8):
        target = f'{i:03}'
        req = dict()
        for j in range(10):
            req[f'{j}'] = 0
        for x in target:
            req[x] += 1
        good = True
        for j in range(10):
            if num_count[f'{j}'] < req[f'{j}']:
                good = False
                break
        if good:
            print('Yes')
            return

    print('No')

if __name__ == '__main__':
    main()
