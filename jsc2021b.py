def main():
    n, m = map(int, input().split())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))

    app = dict()
    for x in a:
        if x not in app:
            app[x] = 1
        else:
            app[x] += 1


    for x in b:
        if x not in app:
            app[x] = 1
        else:
            app[x] += 1

    ans = []
    for x in app:
        if app[x] == 1:
            ans.append(x)

    ans.sort()

    print(' '.join([str(x) for x in ans]))

if __name__ == '__main__':
    main()
