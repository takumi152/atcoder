def main():
    n = int(input())
    a = [0 for _ in range(n)]
    for i in range(n):
        a[i] = int(input())

    appearance = set()
    for x in a:
        if x in appearance:
            appearance.remove(x)
        else:
            appearance.add(x)

    print(len(appearance))

if __name__ == '__main__':
    main()
