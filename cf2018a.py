
def main():
    buf = input()
    N = int(buf)
    count = 0
    for i in range(100):
        if (i+1) % N != 0:
            count += 1
    print(count)

if __name__ == '__main__':
    main()
