def main():
    s = input()
    t = input()

    s = ''.join(sorted(list(s)))
    t = ''.join(reversed(sorted(list(t))))

    if s < t:
        print('Yes')
    else:
        print('No')

    return


if __name__ == '__main__':
    main()
