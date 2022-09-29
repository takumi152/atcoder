def main():
    s = input()

    n = len(s)

    s_rot = [None for _ in range(n)]
    for i in range(n):
        s_rot[i] = s[i:n] + s[0:i]

    s_rot.sort()

    print(s_rot[0])
    print(s_rot[-1])

if __name__ == '__main__':
    main()
