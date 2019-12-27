import random

def main():
    n = 300
    i = 0
    a = []
    while i < n:
        l = random.randint(31, 62)
        b = random.uniform(0.15, 0.55)
        s = ''
        for j in range(l):
            roll = random.random()
            if roll < b:
                s += '1'
            else:
                s += '0'
        if '1' in s:
            a.append(s)
            i += 1
    print(n)
    for s in a:
        print(s)

if __name__ == '__main__':
    main()
