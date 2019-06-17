# TLE

def main():
    buf = input()
    N = int(buf)
    AB = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        AB.append((int(buflist[0]), int(buflist[1])))
    dp = [(factorization(AB[0][0]), factorization(AB[0][1]))]
    for i, x in enumerate(AB):
        if i == 0:
            continue
        last_fac = dp[-1][0] | dp[-1][1]
        a_fac = factorization_set_int(last_fac, x[0])
        b_fac = factorization_set_int(last_fac, x[1])
        dp.append((a_fac, b_fac))
    print(max(max(dp[-1][0]), max(dp[-1][1])))

def factorization(number):
    i = 2
    factor = {1, number}
    while i * i <= number:
        if number % i == 0:
            factor.add(i)
            factor.add(number // i)
        i += 1
    return factor

def factorization_set_int(factor, number):
    new_factor = set()
    for i in factor:
        if number % i == 0:
            new_factor.add(i)
    return new_factor

if __name__ == '__main__':
    main()
