import random

def main():
    print(500)
    s = []
    t = []
    u = []
    v = []
    for i in range(500):
        s.append(random.randint(0, 1))
        t.append(random.randint(0, 1))
        u.append(random.randint(0, (1 << 64) - 1))
        v.append(random.randint(0, (1 << 64) - 1))
    print(' '.join([str(x) for x in s]))
    print(' '.join([str(x) for x in t]))
    print(' '.join([str(x) for x in u]))
    print(' '.join([str(x) for x in v]))

if __name__ == '__main__':
    main()
