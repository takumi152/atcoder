from fractions import Fraction

def main():
    n = int(input())
    xy = [None for _ in range(n)]
    for i in range(n):
        xy[i] = list(map(int, input().split()))

    xy.sort(key = lambda x: x[0])
    for i in range(n-2):
        if xy[i][0] == xy[i+1][0] and xy[i][0] == xy[i+2][0]:
            print('Yes')
            return

    xy.sort(key = lambda x: x[1])
    for i in range(n-2):
        if xy[i][1] == xy[i+1][1] and xy[i][1] == xy[i+2][1]:
            print('Yes')
            return

    xy.sort(key = lambda x: x[0])
    delta = [[None for _ in range(n)] for _ in range(n)]
    for i in range(n):
        for j in range(n):
            if i == j:
                continue
            if xy[i][1] == xy[j][1]:
                continue
            delta[i][j] = Fraction(xy[i][0] - xy[j][0], xy[i][1] - xy[j][1])
    for i in range(n):
        for j in range(n):
            if i == j:
                continue
            if xy[i][1] == xy[j][1]:
                continue
            for k in range(n):
                if i == k or j == k:
                    continue
                if xy[i][1] == xy[k][1] or xy[j][1] == xy[k][1]:
                    continue
                if delta[i][j] == delta[i][k]:
                    print('Yes')
                    return

    print('No')

if __name__ == '__main__':
    main()
