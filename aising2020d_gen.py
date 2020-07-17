import random

def main():
    n = 200000
    s = []
    for i in range(n):
        s.append(str(random.randint(0, 1)))

    print(n)
    print(''.join(s))

if __name__ == '__main__':
    main()
