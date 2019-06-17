
def main():
    buf = input()
    N = int(buf)
    s = []
    for i in range(N):
        buf = input()
        s.append(buf)
    ab_count = 0
    for i in range(N):
        for j in range(len(s[i])-1):
            if s[i][j] == 'A' and s[i][j+1] == 'B':
                ab_count += 1
    A_count = 0
    B_count = 0
    BA_count = 0
    for i in range(N):
        if s[i][-1] == 'A' and s[i][0] == 'B':
            BA_count += 1
        elif s[i][-1] == 'A':
            A_count += 1
        elif s[i][0] == 'B':
            B_count += 1
    # BA-BA
    ab_count += max(0, BA_count - 1)
    # A-BA-B
    if BA_count > 0 and A_count > 0:
        A_count -= 1
        ab_count += 1
    if BA_count > 0 and B_count > 0:
        B_count -= 1
        ab_count += 1
    # A-B
    ab_count += min(B_count, A_count)
    print(ab_count)


if __name__ == '__main__':
    main()
