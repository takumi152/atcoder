
def main():
    buf = input()
    n = int(buf)

    best = 0
    best_num = 1
    for i in range(n):
        k = i + 1
        c = 0
        while k % 2 == 0:
            k //= 2
            c += 1
        if c > best:
            best = c
            best_num = i + 1

    print(best_num)

if __name__ == '__main__':
    main()
