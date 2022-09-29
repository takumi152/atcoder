def main():
    s = input()

    for i, x in enumerate(s):
        if i % 2 == 0 and x in 'abcdefghijklmnopqrstuvwxyz'.upper():
            print('No')
            return
        elif i % 2 == 1 and x in 'abcdefghijklmnopqrstuvwxyz'.lower():
            print('No')
            return

    print('Yes')

if __name__ == '__main__':
    main()
