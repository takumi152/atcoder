
def main():
    buf = input()
    buflist = list(buf.split())
    M = int(buflist[0])
    K = int(buflist[1])
    numbers = []
    if K == 0:
        for i in range(2 ** M):
            numbers.append(i)
            numbers.append(i)
        print(' '.join(map(str, numbers)))
        return
    if K > 2 ** M - 1:
        print(-1) # impossible
        return
    xor_result = 0
    for i in range(2 ** M):
        if i == K:
            continue
        xor_result = xor_result ^ i
    if xor_result != K:
        print(-1) # impossible
        return
    for i in range(2 ** M):
        if i != K:
            numbers.append(i)
    numbers.append(K)
    for i in range(2 ** M - 1, -1, -1):
        if i != K:
            numbers.append(i)
    numbers.append(K)
    print(' '.join(map(str, numbers)))


if __name__ == '__main__':
    main()
