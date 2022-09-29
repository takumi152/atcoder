import math

def main():
    n = int(input())
    x0, y0 = map(int, input().split())
    x12, y12 = map(int, input().split())

    xc, yc = (x0 + x12) / 2, (y0 + y12) / 2

    r = math.sqrt((xc - x0) ** 2 + (yc - y0) ** 2)
    d0 = math.atan2(x0 - xc, y0 - yc)
    d1 = d0 - math.pi / (n / 2)

    x1, y1 = xc + r * math.sin(d1), yc + r * math.cos(d1)

    print(x1, y1)

if __name__ == '__main__':
    main()
