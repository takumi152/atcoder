import random

def main():
    n = 500
    p = [i+1 for i in range(n*n)]
    random.shuffle(p)
    print(n)
    print(' '.join([str(x) for x in p]))

if __name__ == '__main__':
    main()
