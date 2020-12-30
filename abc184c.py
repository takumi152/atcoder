def main():
    r1, c1 = map(int, input().split())
    r2, c2 = map(int, input().split())

    ans = 100
    for i in range(-12, 13):
        for j in range(-12, 13):
            score = (abs(i) + abs(j) + 2) // 3
            if (r1+i, c1+j) == (r2, c2):
                score += 0
            elif (r1+i) + (c1+j) == r2 + c2 or (r1+i) - (c1+j) == r2 - c2:
                score += 1
            elif ((r1+i) + (c1+j)) % 2 == (r2 + c2) % 2:
                score += 2
            else:
                continue
            if score < ans:
                ans = score

    print(ans)

if __name__ == '__main__':
    main()
