
def main():
    buf = input()
    N = int(buf)
    buf = input()
    S = buf
    A_only = True
    B_only = True
    for i in S:
        if i == 'A':
            B_only = False
        if i == 'B':
            A_only = False
    if A_only:
        print(len(S) * N * (len(S) * N + 1) // 2)
        return
    elif B_only:
        print(0)
        return
    first_loop_damage = 0
    power = 1
    for i in S:
        if i == 'A':
            first_loop_damage += power
            power += 1
        if i == 'B':
            power = 1
    second_loop_damage = 0
    for i in S:
        if i == 'A':
            second_loop_damage += power
            power += 1
        if i == 'B':
            power = 1
    print(first_loop_damage + second_loop_damage * (N - 1))


if __name__ == '__main__':
    main()
