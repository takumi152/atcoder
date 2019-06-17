
def main():
    buf = input()
    N = int(buf)
    buf = input()
    A = buf
    buf = input()
    B = buf
    buf = input()
    C = buf
    sub_count = 0
    for i in range(N):
        if A[i] == B[i] and B[i] == C[i] and C[i] == A[i]:
            pass
        elif A[i] == B[i] or B[i] == C[i] or C[i] == A[i]:
            sub_count += 1
        else:
            sub_count += 2
    print(sub_count)

if __name__ == '__main__':
    main()
