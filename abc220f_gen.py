import random

def main():
    n = 200000
    ab = [None for _ in range(n-1)]
    for i in range(n-1):
        # ab[i] = ((i+2) // 2, i+2)
        # ab[i] = (i+1, i+2)
        # ab[i] = (1, i+2)
        ab[i] = (random.randint(1, i+1), i+2)

    print(n)
    for x, y in ab:
        print(x, y)

if __name__ == '__main__':
    main()
