
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    X = int(buflist[1])
    a = []
    b = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        a.append(int(buflist[0]))
        b.append(int(buflist[1]))
    b_max_index = 0
    b_max_value = 0
    for i in range(N):
        if b[i] > b_max_value:
            b_max_index = i
            b_max_value = b[i]
    a[b_max_index] += X
    total = 0
    for i in range(N):
        total += a[i] * b[i]
    print(total)

if __name__ == '__main__':
    main()
