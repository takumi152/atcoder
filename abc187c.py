def main():
    n = int(input())
    s = [None for _ in range(n)]
    for i in range(n):
        s[i] = input()

    yes = set()
    no = set()
    for x in s:
        if x[0] == '!':
            no.add(x[1:])
        else:
            yes.add(x)

    for x in yes:
        if x in no:
            print(x)
            return

    print('satisfiable')


if __name__ == '__main__':
    main()
