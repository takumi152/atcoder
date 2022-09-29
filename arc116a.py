def main():
    t = int(input())
    n = [0 for _ in range(t)]
    for i in range(t):
        n[i] = int(input())

    ans = ['' for _ in range(t)]
    for i in range(t):
        if n[i] % 4 == 0:
            ans[i] = 'Even'
        elif n[i] % 2 == 0:
            ans[i] = 'Same'
        else:
            ans[i] = 'Odd'

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
