
def main():
    buf = input()
    N = int(buf)
    a = []
    for i in range(N):
        buf = input()
        a.append(int(buf))
    for i in a:
        if i % 2 == 0:
            continue
        else:
            print("first")
            return
    print("second")

if __name__ == '__main__':
    main()
