def main():
    k = int(input())
    s = input()

    n = len(s)
    if n <= k:
        print(s)
    else:
        print(s[0:k] + '...')

if __name__ == '__main__':
    main()
