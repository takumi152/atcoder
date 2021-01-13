def main():
    n = int(input())
    s = input()

    t = ''
    for i in range(n):
        t_next = ''
        for x in t:
            if x != s[i]:
                t_next += x
        t_next += s[i]
        t = t_next

    print(t)

if __name__ == '__main__':
    main()
