
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    A = list(reversed(sorted(A)))
    diff = 0
    for i in range(len(A)):
        if i % 2 == 0:
            diff += A[i]
        else:
            diff -= A[i]
    print(diff)

if __name__ == '__main__':
    main()
