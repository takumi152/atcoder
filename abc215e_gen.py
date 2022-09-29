import random

def main():
    n = 1000
    s = random.choices('ABCDEFGHIJ', k=n)

    print(n)
    print(''.join(s))

if __name__ == '__main__':
    main()
