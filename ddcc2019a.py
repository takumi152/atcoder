

def main():
    buf = input()
    N = int(buf)
    chip = 1
    for i in range(N):
        chip *= 4
    print(chip)


if __name__ == '__main__':
    main()
