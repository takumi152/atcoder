def main():
    s = input()

    n = len(s)
    for i in range(n // 2):
        s = s[:-2]
        if s[0:n//2-i-1] == s[n//2-i-1:n-i*2-2]:
            print(n-i*2-2)
            return

if __name__ == '__main__':
    main()
