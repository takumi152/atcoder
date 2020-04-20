def main():
    h, w = map(int, input().split())
    a = [input() for _ in range(h)]

    need1 = 0
    need2 = 0
    need4 = 0
    for i in range(h):
        for j in range(w):
            if (h % 2 == 1 and i * 2 + 1 == h) and (w % 2 == 1 and j * 2 + 1 == w):
                need1 += 1
            elif (h % 2 == 1 and i * 2 + 1 == h) or (w % 2 == 1 and j * 2 + 1 == w):
                need2 += 1
            else:
                need4 += 1

    count = dict()
    for x in a:
        for y in x:
            if y not in count:
                count[y] = 1
            else:
                count[y] += 1

    for _, num in count.items():
        while num >= 4 and need4 > 0:
            num -= 4
            need4 -= 4
        while num >= 2 and need2 > 0:
            num -= 2
            need2 -= 2
        while num >= 1 and need1 > 0:
            num -= 1
            need1 -= 1

    if need4 == 0 and need2 == 0 and need1 == 0:
        print('Yes')
    else:
        print('No')

if __name__ == '__main__':
    main()
