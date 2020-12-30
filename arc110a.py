def main():
    n = int(input())

    mul = [1,1,2,3,2,5,1,7,2,3,1,11,1,13,1,1,2,17,1,19,1,1,1,23,1,5,1,3,1,29,1]

    ans = 1
    for i in range(2, n+1):
        ans *= mul[i]

    ans += 1
    print(ans)

if __name__ == '__main__':
    main()
