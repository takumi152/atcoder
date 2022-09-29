def main():
    k = int(input())
    a, b = input().split()

    a, b = int(a, base=k), int(b, base=k)

    print(a * b)

if __name__ == '__main__':
    main()
