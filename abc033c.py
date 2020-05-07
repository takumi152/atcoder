def main():
    s = input()

    ans = 0
    zeroExist = False
    for x in s:
        if x == '0':
            zeroExist = True
        elif x == '+':
            if not zeroExist:
                ans += 1
            zeroExist = False
    if not zeroExist:
        ans += 1

    print(ans)

if __name__ == '__main__':
    main()
