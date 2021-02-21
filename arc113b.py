def main():
    a, b, c = map(int, input().split())

    p = pow(b, c, 100)
    if p == 0:
        p = 100
    print(pow(a, p, 10))

if __name__ == '__main__':
    main()
