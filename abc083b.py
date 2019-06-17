
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    A = int(buflist[1])
    B = int(buflist[2])
    sum = 0
    for i in range(1, N + 1):
        count = 0
        for d in [int(d) for d in str(i)]:
            count += d
        if count >= A and count <= B:
            sum += i
    print(sum)

if __name__ == '__main__':
    main()
