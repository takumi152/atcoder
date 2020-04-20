def main():
    n = int(input())
    s = input()

    left = 0
    right = 0
    for c in s:
        if c == '(':
            right += 1
        else:
            if right > 0:
                right -= 1
            else:
                left += 1

    print('(' * left + s + ')' * right)

if __name__ == '__main__':
    main()
