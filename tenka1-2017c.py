def main():
    buf = input()
    N = int(buf)
    if N % 2 == 0:
        print(N // 2, N, N)
        return
    else:
        divisor = N
        mult = 1
        while True:
            factor = factorization(divisor)
            for h in factor:
                for n in factor:
                    for w in factor:
                        if h + n + w == 4 * mult:
                            print(divisor // h, divisor // n, divisor // w)
                            return
            divisor += N
            mult += 1

def factorization(number):
    i = 2
    factor = {1, number}
    while i * i <= number:
        if number % i == 0:
            factor.add(i)
            factor.add(number // i)
        i += 1
    return factor

if __name__ == '__main__':
    main()
