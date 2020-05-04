import math

def main():
    n = int(input())

    print(min(100 * math.ceil(n / 10), 100 * (n // 10) + 15 * (n % 10)))

if __name__ == '__main__':
    main()
