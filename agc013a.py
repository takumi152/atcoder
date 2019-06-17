
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    split_count = 1
    increasing = None
    for i in range(1, N):
        if increasing == None:
            if A[i-1] < A[i]:
                increasing = True
            elif A[i-1] > A[i]:
                increasing = False
            # unsure if A[i-1] == A[i]
        elif increasing and A[i-1] > A[i]:
            split_count += 1
            increasing = None # unsure if it increases or not at this point
        elif not increasing and A[i-1] < A[i]:
            split_count += 1
            increasing = None # unsure if it increases or not at this point
    print(split_count)

if __name__ == '__main__':
    main()
