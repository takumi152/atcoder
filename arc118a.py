def main():
    t, n = map(int, input().split())

    price = 0
    cent = 0
    loop_count = 0
    while True:
        price += 1
        cent += t
        if cent >= 100:
            price += 1
            cent -= 100
            loop_count += 1
        if cent == 0:
            break

    full_loop_count = (n - 1) // loop_count
    base_price = price * full_loop_count
    n -= full_loop_count * loop_count

    price = base_price
    cent = 0
    while True:
        price += 1
        cent += t
        if cent >= 100:
            cent -= 100
            n -= 1
            if n == 0:
                print(price)
                return
            else:
                price += 1

if __name__ == '__main__':
    main()
