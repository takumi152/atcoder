def main():
    a = int(input())
    b = int(input())

    if b % 2 == 0:
        print(str(b // 2) + '0' + str(a))
    elif b > 1:
        print(str(b // 2) + '5' + str(a))
    else:
        print('5' + str(a))

if __name__ == '__main__':
    main()
