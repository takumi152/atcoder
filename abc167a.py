def main():
    s = input()
    t = input()

    n = len(s)
    for i in range(n):
        if s[i] != t[i]:
            print('No')
            return

    print('Yes')

if __name__ == '__main__':
    main()
