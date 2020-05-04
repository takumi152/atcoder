import random

def main():
    n = 100000
    p = ['AB', 'AC', 'BC']
    a = 1
    b = 1
    c = 0
    print(n, a, b, c)
    s = []
    for i in range(n):
        s.append(p[random.randint(0, 2)])
    for x in s:
        print(x)

if __name__ == '__main__':
    main()
