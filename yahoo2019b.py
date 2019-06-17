
def main():
    a = []
    b = []
    for i in range(3):
        buf = input()
        buflist = buf.split()
        a.append(int(buflist[0]))
        b.append(int(buflist[1]))
    count = [0, 0, 0, 0]
    for i in a:
        count[i-1] += 1
    for i in b:
        count[i-1] += 1
    for i in count:
        if i >= 3:
            print("NO")
            return
    print("YES")

if __name__ == '__main__':
    main()
