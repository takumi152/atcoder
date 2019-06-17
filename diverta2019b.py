
def main():
    buf = input()
    buflist = buf.split()
    R = int(buflist[0])
    G = int(buflist[1])
    B = int(buflist[2])
    N = int(buflist[3])
    combination = 0
    for i in range(3001):
        if i * R > N:
            break
        for j in range(3001):
            if i * R + j * G > N:
                break
            if (N - (i * R + j * G)) % B == 0:
                combination += 1
    print(combination)


if __name__ == '__main__':
    main()
