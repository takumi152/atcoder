def main():
    s = input()
    t = input()

    n = len(s)

    charmap = dict()
    usedchar = set()

    for i in range(n):
        if s[i] not in charmap:
            if t[i] in usedchar:
                print('No')
                return
            charmap.update({s[i]: t[i]})
            usedchar.add(t[i])
        elif charmap[s[i]] != t[i]:
            print('No')
            return

    print('Yes')
    return


if __name__ == '__main__':
    main()
