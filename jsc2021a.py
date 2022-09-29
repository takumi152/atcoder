from fractions import Fraction

def main():
    x, y, z = map(int, input().split())

    v_target = Fraction(y, x)

    ans = 0
    for i in range(10000000):
        v = Fraction(i, z)
        if v < v_target:
            ans = i
        else:
            break

    print(ans)

if __name__ == '__main__':
    main()
