
MOD = int(1e9) + 7

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    a = []
    for i in range(M):
        buf = input()
        a.append(int(buf))
    a.append(N+9999) # sentinel
    pattern = [1]
    for i in range(1, N+10):
        pattern.append(0)
    pointer = 0
    for i in range(0, N+2):
        if a[pointer] == i:
            pattern[i] = 0
            pointer += 1
        pattern[i+1] = (pattern[i+1] + pattern[i]) % MOD
        pattern[i+2] = (pattern[i+2] + pattern[i]) % MOD
    print(pattern[N])

if __name__ == '__main__':
    main()
