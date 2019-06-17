
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    P = int(buflist[1])
    factor = prime_factorization(P)
    max_gcd = 1
    for f, c in factor.items():
        while c >= N:
            max_gcd *= f
            c -= N
    print(max_gcd)

def prime_factorization(number):
    n = number
    i = 2
    factor = {}
    while i * i <= n:
        while n % i == 0:
            if i in factor:
                factor[i] += 1
            else:
                factor.update({i : 1})
            n //= i
        i += 1
    if n > 1 or not factor:
        if n in factor:
            factor[n] += 1
        else:
            factor.update({n : 1})
    return factor

if __name__ == '__main__':
    main()
