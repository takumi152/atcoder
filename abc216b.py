def main():
    s = set()
    for i in range(3):
        t = input()
        s.add(t)

    contests = {'ABC', 'ARC', 'AGC', 'AHC'}
    diff = contests - s

    for x in diff:
        print(x)
        break

if __name__ == '__main__':
    main()
