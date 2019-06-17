
def main():
    buf = input()
    c500 = int(buf)
    buf = input()
    c100 = int(buf)
    buf = input()
    c50 = int(buf)
    buf = input()
    target = int(buf)
    count = 0
    for u500 in range(c500 + 1):
        if u500 * 500 > target:
            break
        elif u500 * 500 + c100 * 100 + c50 * 50 < target:
            continue
        else:
            for u100 in range(c100 + 1):
                if u500 * 500 + u100 * 100 > target:
                    break
                elif u500 * 500 + u100 * 100 + c50 * 50 < target:
                    continue
                else:
                    for u50 in range(c50 + 1):
                        if u500 * 500 + u100 * 100 + u50 * 50 > target:
                            break
                        elif u500 * 500 + u100 * 100 + u50 * 50 == target:
                            count += 1
    print(count)

if __name__ == '__main__':
    main()
