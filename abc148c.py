import fractions

def main():
    buf = input()
    buflist = buf.split()
    a = int(buflist[0])
    b = int(buflist[1])

    ans = a * b // fractions.gcd(a, b)

    print(ans)

if __name__ == '__main__':
    main()
