
def main():
    buf = input()
    buflist = buf.split()
    r = int(buflist[0])
    D = int(buflist[1])
    x2000 = int(buflist[2])
    x = x2000
    for i in range(10):
        x = r * x - D
        print(x)

if __name__ == '__main__':
    main()
