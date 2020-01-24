
def main():
    buf = input()
    n = int(buf)
    a = []
    for i in range(n):
        buf = input()
        a.append(int(buf))

    found = set()
    dupe = None
    for i in range(n):
        if a[i] in found:
            dupe = a[i]
        else:
            found.add(a[i])

    if dupe is None:
        print('Correct')
        return

    for i in range(1, n+1):
        if i not in found:
            print(dupe, i)
            return

if __name__ == '__main__':
    main()
