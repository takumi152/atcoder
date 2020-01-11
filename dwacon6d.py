# stupid

import itertools

def main():
    n = int(input())
    x = list(map(int, input().split()))
    a = []
    for i in range(len(x)):
        a.append(i)
        x[i] -= 1
    for y in itertools.permutations(a):
        good = True
        for i in range(len(x)-1):
            if y[i+1] == x[y[i]]:
                good = False
                break
        if good:
            print(' '.join([str(t+1) for t in y]))
            return

if __name__ == '__main__':
    main()
