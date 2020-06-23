def main():
    n = int(input())
    s = ['' for _ in range(5)]
    for i in range(5):
        s[i] = input()

    ref = ['.###..#..###.###.#.#.###.###.###.###.###.',
           '.#.#.##....#...#.#.#.#...#.....#.#.#.#.#.',
           '.#.#..#..###.###.###.###.###...#.###.###.',
           '.#.#..#..#.....#...#...#.#.#...#.#.#...#.',
           '.###.###.###.###...#.###.###...#.###.###.']
    ans = ''
    for i in range(n):
        for j in range(10):
            match = True
            for k in range(5):
                if not match:
                    break
                for l in range(4):
                    if s[k][i*4+l] != ref[k][j*4+l]:
                        match = False
                        break
            if match:
                ans += str(j)
                break

    print(ans)

if __name__ == '__main__':
    main()
