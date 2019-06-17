
def main():
    buf = input()
    N = int(buf)
    buf = input()
    H = int(buf)
    buf = input()
    W = int(buf)
    hspace = N - H + 1
    wspace = N - W + 1
    print(hspace * wspace)

if __name__ == '__main__':
    main()
