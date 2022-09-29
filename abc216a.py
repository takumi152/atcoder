def main():
    x = input()

    if x[1] == '.':
        if int(x[2]) <= 2:
            print(x[0:1] + '-')
        elif int(x[2]) <= 6:
            print(x[0:1])
        else:
            print(x[0:1] + '+')
    else:
        if int(x[3]) <= 2:
            print(x[0:2] + '-')
        elif int(x[3]) <= 6:
            print(x[0:2])
        else:
            print(x[0:2] + '+')


if __name__ == '__main__':
    main()
