import fractions

def lcm(a, b):
    return a * b // fractions.gcd(a, b)

def main():
    n = int(input())
    t = []
    for i in range(n):
        t.append(int(input()))

    ans = 1
    for i in range(n):
        ans = lcm(ans, t[i])

    print(ans)

if __name__ == '__main__':
    main()
