# WA

def main():
    buf = input()
    N = int(buf)
    buf = input()
    S = buf
    block = []
    white_count = 0
    black_count = 0
    white_swap_count = 0
    black_swap_count = 0
    minimum = 9999999999999
    count = 0
    is_black = False
    for i in S:
        if i == '#':
            if not is_black:
                is_black = True
                block.append(count)
                count = 0
            black_count += 1
        elif i == '.':
            if is_black:
                is_black = False
                block.append(count)
                count = 0
            white_count += 1
        count += 1
    block.append(count)
    minimum = white_count
    is_black = False
    for i, x in enumerate(block):
        if i % 2 == 0:
            white_count -= x
            if i != 0:
                white_swap_count += x
        else:
            black_count -= x
            swap = black_swap_count + white_count
            black_swap_count += x
            if swap < minimum:
                minimum = swap
    print(minimum)

if __name__ == '__main__':
    main()
