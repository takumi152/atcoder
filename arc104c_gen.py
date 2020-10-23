import random

def main():
    n = 100
    r = []
    p = 0
    while p < n:
        r.append(random.randint(1, n-p))
        p += r[-1]
    random.shuffle(r)

    print(n)
    d = 1
    for x in r:
        for i in range(x):
            print((-1 if random.random() < 0.3 else d+i), (-1 if random.random() < 0.3 else d+i+x))
        d += x * 2

if __name__ == '__main__':
    main()
