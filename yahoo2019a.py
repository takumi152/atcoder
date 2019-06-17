
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    K = int(buflist[1])
    count = 0
    for i in range(0, N, 2):
        count += 1
    if count >= K:
        print("YES")
    else:
        print("NO")

if __name__ == '__main__':
    main()
