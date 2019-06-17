
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    A = int(buflist[1])-1
    B = int(buflist[2])-1
    C = int(buflist[3])-1
    D = int(buflist[4])-1
    buf = input()
    S = buf
    if D < C:
        space = 0
        for i in range(B-1, min(D+2, N)):
            if S[i] == '.':
                space += 1
                if space >= 3:
                    break
            else:
                space = 0
        if space < 3:
            print('No') # unswappable
            return
    rock_count = 0
    for i in range(A, max(C, D)+1):
        if S[i] == '#':
            rock_count += 1
            if rock_count >= 2:
                break
        else:
            rock_count = 0
    if rock_count >= 2:
        print('No')
    else:
        print('Yes')

if __name__ == '__main__':
    main()
