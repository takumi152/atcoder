def main():
    n = int(input())
    a = list(map(int, input().split()))

    section_needed = sum(a)
    ans = 0
    section_count = 1
    total_section = 1
    for i in range(n+1):
        ans += section_count
        section_count -= a[i]
        if section_count <= 0 and i != n:
            print(-1)
            return
        if i == n and total_section != section_needed:
            print(-1)
            return
        next_branch = min(max(0, section_needed - total_section), section_count)
        total_section += next_branch
        section_count += next_branch

    print(ans)

if __name__ == '__main__':
    main()
