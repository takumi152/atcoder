def main():
    n = int(input())
    s = [None for _ in range(n)]
    for i in range(n):
        s[i] = input()

    vote_count = dict()
    for x in s:
        if x not in vote_count:
            vote_count[x] = 1
        else:
            vote_count[x] += 1

    best_name = None
    best_vote = 0
    for k, v in vote_count.items():
        if v > best_vote:
            best_name = k
            best_vote = v

    print(best_name)

if __name__ == '__main__':
    main()
