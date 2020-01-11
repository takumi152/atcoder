import random

def main():
    n = 3
    x = []
    for i in range(n):
        x.append(i)
        while True:
            x[i] = random.randrange(0, n)
            if x[i] != i:
                break
    print(n)
    print(' '.join([str(t+1) for t in x]))
    return

if __name__ == '__main__':
    main()
