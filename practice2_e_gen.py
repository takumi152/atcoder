import random

def main():
    n = 30
    k = 3
    a_min = 1
    a_max = 10 ** 5

    a = [[random.randint(a_min, a_max) for _ in range(n)] for _ in range(n)]

    print(n, k)
    for i in range(n):
        print(' '.join([str(x) for x in a[i]]))

if __name__ == '__main__':
    main()
