
def main():
    buf = input()
    s = buf

    n = len(s)
    a = []
    last = 0
    for i in range(n):
        if i == last:
            continue
        if s[i].isupper():
            a.append(s[last:i+1])
            last = i+1

    for i in range(len(a)):
        a[i] = a[i].lower()
    a.sort()
    for i in range(len(a)):
        a[i] = list(a[i])
        a[i][0] = a[i][0].upper()
        a[i][-1] = a[i][-1].upper()
        a[i] = ''.join([x for x in a[i]])

    print(''.join(a))

if __name__ == '__main__':
    main()
