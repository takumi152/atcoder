
def main():
    buf = input()
    buflist = buf.split()
    K = int(buflist[0])
    A = int(buflist[1])
    B = int(buflist[2])
    biscuit = 1
    if A >= B - 2:
        biscuit += K
    else:
        if K <= A - 1:
            biscuit += K
        else:
            biscuit += A - 1
            remaining = K - (A - 1)
            buysell_count = remaining // 2
            biscuit += (B - A) * buysell_count
            remaining -= buysell_count * 2
            if remaining == 1:
                biscuit += 1
    print(biscuit)

if __name__ == '__main__':
    main()
