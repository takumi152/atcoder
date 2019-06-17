
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    cakelist = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        cakelist.append(list(map(int, buflist)))
    highest = 0
    for i in range(0b111+1):
        cake = cakelist
        x_factor = 1
        y_factor = 1
        z_factor = 1
        if i & 0b100:
            x_factor = -1
        if i & 0b010:
            y_factor = -1
        if i & 0b001:
            z_factor = -1
        cake = list(reversed(list(sorted(list(map(lambda x:x[0]*x_factor + x[1]*y_factor + x[2]*z_factor, cake))))))
        score = sum(cake[0:M])
        if score > highest:
            highest = score
    print(highest)


if __name__ == '__main__':
    main()
