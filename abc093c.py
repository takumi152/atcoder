
def main():
    a = list(map(int, input().split()))

    ans = 0
    while True:
        a.sort()
        if a[0] == a[1] and a[1] == a[2]:
            break
        elif a[1] < a[2]:
            a[0] += 1
            a[1] += 1
        else:
            a[0] += 2
        ans += 1

    print(ans)

if __name__ == '__main__':
    main()
