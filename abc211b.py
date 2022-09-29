def main():
    s = [None for _ in range(4)]
    for i in range(4):
        s[i] = input()

    if 'H' in s and '2B' in s and '3B' in s and 'HR' in s:
        print('Yes')
    else:
        print('No')

if __name__ == '__main__':
    main()
