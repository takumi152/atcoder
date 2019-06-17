
def main():
    buf = input()
    S = buf
    target = 753
    min_diff = 999
    for i in range(len(S) - 2):
        num = int(S[i:i+3])
        diff = abs(num - target)
        if diff < min_diff:
            min_diff = diff
    print(min_diff)


if __name__ == '__main__':
    main()
