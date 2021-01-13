def main():
    n = int(input())
    s = ['' for _ in range(n)]
    for i in range(n):
        s[i] = input()

    t = [None for _ in range(n)]
    for i in range(n):
        t[i] = [int(s[i]), 0]
        for x in s[i]:
            if x == '0':
                t[i][1] -= 1
            else:
                break

    idx = [i for i in range(n)]
    idx.sort(key = lambda x: t[x])

    for i in range(n):
        print(s[idx[i]])

if __name__ == '__main__':
    main()
