
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

def main():
    n = int(input())

    if n == 1:
        print(0)
        return

    prime_factor = prime_factorization(n)
    p = len(prime_factor)
    prime_count = [0 for _ in range(p)]
    for i, x in enumerate(prime_factor.items()):
        prime_count[i] = x[1]

    ans = 0
    for i in range(p):
        need = 1
        while prime_count[i] >= need:
            prime_count[i] -= need
            ans += 1
            need += 1

    print(ans)


if __name__ == '__main__':
    main()
