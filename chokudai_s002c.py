
def main():
    buf = input()
    N = int(buf)
    AB = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        AB.append((int(buflist[0]), int(buflist[1])))
    maximum = 0
    for x in AB:
        pair_sum = x[0] + x[1]
        if pair_sum > maximum:
            maximum = pair_sum
    print(maximum)

if __name__ == '__main__':
    main()
