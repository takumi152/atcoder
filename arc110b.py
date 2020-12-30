def main():
    n = int(input())
    t = input()

    if n == 1:
        if t == '0':
            print(1 * 10 ** 10)
        else:
            print(2 * 10 ** 10)
    elif n == 2:
        if t in ('10', '11'):
            print(1 * 10 ** 10)
        elif t == '01':
            print(1 * 10 ** 10 - 1)
        else:
            print(0)
    else:
        good = True
        count_0 = 0
        count_1 = 0
        for i in range(3):
            for j in range(i, n-3, 3):
                if t[j] != t[j+3]:
                    good = False
                    break
            if not good:
                break
            if t[i] == '0':
                count_0 += 1
            else:
                count_1 += 1
        if not good:
            print(0)
        elif not (count_0 == 1 and count_1 == 2):
            print(0)
        else:
            ans = 10 ** 10
            for i in range(n-1):
                if t[i] == '0' and t[i+1] == '1':
                    ans -= 1
            print(ans)

if __name__ == '__main__':
    main()
