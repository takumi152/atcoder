
def main():
    buf = input()
    buflist = buf.split()
    A = int(buflist[0])
    B = int(buflist[1])
    C = int(buflist[2])
    len_list = list(sorted([A,B,C]))
    if len_list == [5,5,7]:
        print("YES")
    else:
        print("NO")

if __name__ == '__main__':
    main()
