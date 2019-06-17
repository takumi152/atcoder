
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    BC = []
    for i in range(M):
        buf = input()
        buflist = buf.split()
        BC.append((int(buflist[0]), int(buflist[1])))
    A = list(sorted(A))
    BC = list(sorted(BC, reverse=True, key=lambda x:x[1]))
    pointer = 0
    for bc in BC:
        remaining = bc[0]
        substitute_to = bc[1]
        if pointer >= N:
            break
        if A[pointer] >= substitute_to:
            break
        while remaining > 0 and pointer < N:
            if A[pointer] >= substitute_to:
                break
            A[pointer] = substitute_to
            pointer += 1
            remaining -= 1
    print(sum(A))

if __name__ == '__main__':
    main()
