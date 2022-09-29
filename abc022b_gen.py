import random

def main():
    n = 1000
    a = [0 for _ in range(n)]
    for i in range(n):
        a[i] = random.randint(1, 100000)

    print(n)
    for x in a:
        print(x)

if __name__ == '__main__':
    main()
