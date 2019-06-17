
def main():
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    max_score = 0
    if A[1] + A[3] <= A[0] and A[2] + A[4] > max_score:
        max_score = A[2] + A[4]
    if A[3] <= A[0] and A[4] > max_score:
        max_score = A[4]
    if A[1] <= A[0] and A[2] > max_score:
        max_score = A[2]
    print(max_score)

if __name__ == '__main__':
    main()
