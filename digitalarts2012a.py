def main():
    s = input().split()
    n = int(input())
    t = [input() for _ in range(n)]

    for i, x in enumerate(s):
        for y in t:
            if len(x) != len(y):
                continue
            match = True
            for k, _ in enumerate(x):
                if x[k] != y[k] and y[k] != '*':
                    match = False
                    break
            if match:
                s[i] = '*' * len(x)
                break

    print(' '.join(s))

if __name__ == '__main__':
    main()
