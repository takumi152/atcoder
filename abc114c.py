
def main():
    buf = input()
    N = int(buf)
    table = []
    for i in range(1, 10):
        create_table("", 0, i, table)
    count = 0
    for i in table:
        if i <= N:
            count += 1
        else:
            break
    print(count)

def create_table(num, current_digit, maximum_digit, table):
    if current_digit < maximum_digit:
        create_table(num + "3", current_digit + 1, maximum_digit, table)
        create_table(num + "5", current_digit + 1, maximum_digit, table)
        create_table(num + "7", current_digit + 1, maximum_digit, table)
    else:
        appearance = [0, 0, 0]
        for _, x in enumerate(num):
            if x == "3":
                appearance[0] += 1
            elif x == "5":
                appearance[1] += 1
            elif x == "7":
                appearance[2] += 1
            else:
                return
        if min(appearance) >= 1:
            table.append(int(num))
        return

if __name__ == '__main__':
    main()
