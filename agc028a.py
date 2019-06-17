
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    buf = input()
    S = buf
    buf = input()
    T = buf
    L = lcm(N, M)
    # main part
    i = 0
    j = 0
    while i < len(S) and j < len(T):
        if i * (L / N) == j * (L / M):
            if not S[i] == T[j]:
                print(-1) # faliure
                return
            else:
                i += 1
                j += 1
        elif i * (L / N) < j * (L / M):
            i += 1
        elif i * (L / N) > j * (L / M):
            j += 1
    print(L) # success
    return

# 最大公約数 & 最小公倍数
def gcd(a, b):
    while b > 0:
        a, b = b, a % b
    return int(a)

def lcm(a, b):
    return int(a * b / gcd(a, b))

if __name__ == '__main__':
    main()
