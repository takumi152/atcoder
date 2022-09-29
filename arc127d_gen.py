import random

def main():
    n = 12345
    a = [random.randrange(0, 2 ** 18) for _ in range(n)]
    b = [random.randrange(0, 2 ** 18) for _ in range(n)]

    print(n)
    print(' '.join([str(x) for x in a]))
    print(' '.join([str(x) for x in b]))

if __name__ == '__main__':
    main()
