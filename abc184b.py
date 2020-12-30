def main():
    n, x = map(int, input().split())
    s = input()

    score = x
    for i in range(n):
        if s[i] == 'o':
            score += 1
        elif s[i] == 'x' and score > 0:
            score -= 1

    print(score)

if __name__ == '__main__':
    main()
