def main():
    n = input()

    ans = 0
    for i in range(1, 2 ** len(n) - 1):
        a = []
        b = []
        for j in range(len(n)):
            if i & 2 ** j:
                a.append(n[j])
            else:
                b.append(n[j])
        a.sort(reverse=True)
        b.sort(reverse=True)
        ans = max(ans, int(''.join(a)) * int(''.join(b)))

    print(ans)

if __name__ == '__main__':
    main()
