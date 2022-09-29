import random

def main():
    n = 200000
    x = random.choices(range(0, 10 ** 9 + 1), k = n)
    y = random.choices(range(0, 10 ** 9 + 1), k = n)

    print(n)
    print(' '.join([str(z) for z in x]))
    print(' '.join([str(z) for z in y]))

if __name__ == '__main__':
    main()
