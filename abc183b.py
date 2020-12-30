def main():
    sx, sy, gx, gy = map(int, input().split())

    ty = sy + gy
    ans = (sx * (ty - sy) + gx * (ty - gy)) / ty

    print(ans)

if __name__ == '__main__':
    main()
