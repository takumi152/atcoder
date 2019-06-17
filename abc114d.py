import math
import functools

TARGET = 75

def main():
    l = [0]
    for j in range(1, 101):
        f = math.factorial(j)
        primes = prime_factorization(f)
        primes = [x for _, x in primes.items()]
        counts = []
        for i, _ in enumerate(primes):
            counts.append(0)
        l.append(count_divisor(primes, counts, 0, 0))
        print(j)
        print(l, flush=True)
    print(l)

def answer():
    buf = input()
    N = int(buf)
    table = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 3, 3, 8, 8, 11, 11, 11, 11, 14, 14, 32, 32, 35, 35, 35, 35, 42, 42, 42, 42, 49, 49, 49, 49, 49, 49, 75, 75, 86, 86, 86, 86, 86, 86, 123, 123, 131, 131, 131, 131, 148, 148, 153, 153, 170, 170, 170, 170, 170, 170, 227, 227, 227, 227, 227, 227, 250, 250, 323, 323, 324, 324, 324, 324, 354, 354, 354, 354, 384, 384, 384, 384, 384, 391, 491, 491, 529, 529, 529, 529, 529, 529, 543]
    print(table[N])

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

def count_divisor(primes, counts, current, count):
    divisors = functools.reduce(lambda x, y: x*(y+1), counts, 1)
    if divisors >= TARGET:
        if divisors == TARGET:
            return count + 1
        else:
            return count
    new_counts = list(counts)
    for i in range(current, len(counts)):
        if counts[i] >= primes[i]:
            continue
        new_counts[i] += 1
        count = count_divisor(primes, new_counts, i, count)
        new_counts[i] -= 1
    return count

if __name__ == '__main__':
    #main()
    answer()
