import random;

def main():
    n = 500
    m = 400
    a = [random.randint(2, 1000) for _ in range(n)]
    b = [random.randint(2, 1000) for _ in range(n)]
    r1 = random.randint(2, 100000)
    r2 = random.randint(2, 100000)
    r3 = random.randint(2, 100000)
    print(n, m)
    print(' '.join([str(x) for x in a]))
    print(' '.join([str(x) for x in b]))
    print(r1, r2, r3)

if __name__ == '__main__':
    main()
