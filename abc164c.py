def main():
    n = int(input())
    s = [None] * n
    for i in range(n):
        s[i] = input()

    gained = set()
    for x in s:
        gained.add(x)

    print(len(gained))

if __name__ == '__main__':
    main()
