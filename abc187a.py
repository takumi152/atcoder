def main():
    a, b = input().split()

    a_total = int(a[0]) + int(a[1]) + int(a[2])
    b_total = int(b[0]) + int(b[1]) + int(b[2])

    print(max(a_total, b_total))

if __name__ == '__main__':
    main()
