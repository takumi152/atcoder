# WIP

def main():
    buf = input()
    Q = int(buf)
    query = []
    for i in range(Q):
        buf = input()
        buflist = buf.split()
        query.append(tuple(map(int, buflist)))


if __name__ == '__main__':
    main()
