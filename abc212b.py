def main():
    x = input()

    if x[0] == x[1] and x[0] == x[2] and x[0] == x[3]:
        print('Weak')
        return

    for i in range(3):
        if not (int(x[i]) + 1 == int(x[i+1]) or (x[i] == '9' and x[i+1] == '0')):
            print('Strong')
            return

    print('Weak')

if __name__ == '__main__':
    main()
