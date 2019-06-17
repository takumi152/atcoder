def main():
    buf = input()
    N = int(buf)
    if N == 1:
        print("Hello World")
    else:
        buf = input()
        A = int(buf)
        buf = input()
        B = int(buf)
        print(A+B)


if __name__ == '__main__':
    main()
