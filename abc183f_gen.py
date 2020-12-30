import random

def main():
    n = 200000
    q = 199900
    c = [random.randint(1, n) for i in range(n)]
    query = []
    for i in range(q):
        query.append((1, 199998-i, 199999-i))

    print(n, q)
    print(' '.join([str(x) for x in c]))
    for x in query:
        print(' '.join([str(y) for y in x]))

if __name__ == '__main__':
    main()
