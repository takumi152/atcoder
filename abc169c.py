def main():
    a, b = input().split()

    a = int(a)
    if len(b) == 4:
        b = int(b[0] + b[2] + b[3])
    else:
        b = int(b[0] + b[1] + b[3] + b[4])

    print(a * b // 100)

if __name__ == '__main__':
    main()
