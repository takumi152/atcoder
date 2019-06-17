
def main():
    buf = input()
    N = int(buf)
    # N / m = i % i
    # m * i + i = N
    total = 0
    i = 1
    while i * i < N:
        if N % i == 0:
            div = N // i - 1
            if N // div == N % div:
                total += div
        i += 1
    print(total)


if __name__ == '__main__':
    main()
