
MOD = int(1e9) + 7

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    buf = input()
    buflist = buf.split()
    B = list(map(int, buflist))

    A = list(reversed(list(sorted(A))))
    B = list(reversed(list(sorted(B))))
    A.append(0) # sentinel
    B.append(0) # sentinel
    pattern_count = 1
    A_pointer = 0
    B_pointer = 0
    free_count = 0
    for i in range(N * M, 0, -1):
        if i == A[A_pointer] or i == B[B_pointer]:
            if i == A[A_pointer] and i == B[B_pointer]:
                free_count += A_pointer + B_pointer
                A_pointer += 1
                B_pointer += 1
            elif i == A[A_pointer]:
                free_count += B_pointer - 1
                pattern_count *= B_pointer
                A_pointer += 1
            else:
                free_count += A_pointer - 1
                pattern_count *= A_pointer
                B_pointer += 1
        elif free_count == 0:
            print(0) # impossible
            return
        else:
            pattern_count *= free_count
            free_count -= 1
        pattern_count %= MOD
    print(pattern_count)

if __name__ == '__main__':
    main()
