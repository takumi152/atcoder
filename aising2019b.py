
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    A = int(buflist[0])
    B = int(buflist[1])
    buf = input()
    buflist = buf.split()
    P = list(map(int, buflist))
    group_count = [0, 0, 0]
    for i, score in enumerate(P):
        if score <= A:
            group_count[0] += 1
        elif score <= B:
            group_count[1] += 1
        else:
            group_count[2] += 1
    print(min(group_count))

if __name__ == '__main__':
    main()
