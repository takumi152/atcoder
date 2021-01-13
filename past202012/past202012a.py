def main():
    s = input()
    if 'ooo' in (s[0:3], s[1:4], s[2:5]):
        print('o')
    elif 'xxx' in (s[0:3], s[1:4], s[2:5]):
        print('x')
    else:
        print('draw')

if __name__ == '__main__':
    main()
