import decimal
import bisect

def main():
    x, y, r = map(decimal.Decimal, input().split())

    x *= 10 ** 4
    y *= 10 ** 4
    r *= 10 ** 4

    x, y, r = int(x), int(y), int(r)

    left_target = []
    for i in range(0, -(10 ** 9) - 10 ** 4, -(10 ** 4)):
        left_target.append((i - x % 10 ** 4) ** 2)

    right_target = []
    for i in range(0, 10 ** 9 + 10 ** 4, 10 ** 4):
        if i + 10 ** 4 - (x % 10 ** 4) == 0:
            continue
        right_target.append((i + 10 ** 4 - (x % 10 ** 4)) ** 2)

    ans = 0
    for i in range(-2 * 10 ** 9, 2 * 10 ** 9 + 10 ** 4, 10 ** 4):
        lim = r ** 2 - (y - i) ** 2
        ans += bisect.bisect(left_target, lim)
        ans += bisect.bisect(right_target, lim)

    print(ans)

if __name__ == '__main__':
    main()
