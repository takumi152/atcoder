def main():
    n = 100
    a = []
    for i in range(99, -1, -1):
        a.append(i)

    print(n)
    print(' '.join([str(x) for x in a]))

if __name__ == '__main__':
    main()
