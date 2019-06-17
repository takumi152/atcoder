
def main():
    buf = input()
    count = 0
    for i in buf:
        if i == '1':
            count += 1
    print(count)

if __name__ == '__main__':
    main()
