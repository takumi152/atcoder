import math

def main():
    a, b, x = map(int, input().split())

    a, b, x = 1.0, b / a, x / (a * a * b)
    x *= b

    if x * 2.0 > b:
        print(math.atan((b - x) / 0.5) * (180.0 / math.pi))
    else:
        print(90.0 - math.atan(x * 2 / (b * b)) * (180.0 / math.pi))

if __name__ == '__main__':
    main()
