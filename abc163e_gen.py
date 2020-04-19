import random

def main():
    n = 2000
    a = []
    for i in range(n):
        a.append(random.randint(1, int(1e9)))

    print(n)
    print(' '.join([str(x) for x in a]))


if __name__ == '__main__':
    main()
