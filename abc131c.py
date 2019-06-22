import fractions
import math

def main():
    buf = input()
    buflist = buf.split()
    A = int(buflist[0])
    B = int(buflist[1])
    C = int(buflist[2])
    D = int(buflist[3])
    l = (C * D) // fractions.gcd(C, D)
    cA = A // C * C + (0 if A % C == 0 else C)
    cB = B // C * C
    c_count = max(0, (cB - cA) // C + 1)
    dA = A // D * D + (0 if A % D == 0 else D)
    dB = B // D * D
    d_count = max(0, (dB - dA) // D + 1)
    lA = A // l * l + (0 if A % l == 0 else l)
    lB = B // l * l
    l_count = max(0, (lB - lA) // l + 1)
    selected = c_count + d_count - l_count
    unselected = (B - A + 1) - selected
    print(unselected)

if __name__ == '__main__':
    main()
