
def main():
    buf = input()
    S = buf
    total = split_sum(S)
    print(total)

def split_sum(S):
    total = 0
    for i in range(int(2 ** (len(S) - 1))):
        factor = 1
        mask = 0b0000000001
        for j in range(len(S)):
            total += int(S[-j - 1]) * factor
            if i & (mask << j):
                factor = 1
            else:
                factor *= 10
    return total

if __name__ == '__main__':
    main()
