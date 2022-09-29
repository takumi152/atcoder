import random

def main():
    n = int(input())

    if n == 1:
        print(1)
        print(1)
        return

    fib = [1, 1]
    while fib[-1] < 10 ** 18:
        fib.append(fib[-1] + fib[-2])

    x = n
    fib_now = []
    for i in range(len(fib)-1, -1, -1):
        if x >= fib[i]:
            fib_now.append(i)
            x -= fib[i]

    target = [[n, 2]]

    while fib_now:
        for i, _ in enumerate(fib_now):
            fib_now[i] -= 1

        if sum([fib[x] for x in fib_now]) > 1:
            target.append([sum([fib[x] for x in fib_now]), 2])
            target[-2][1] = 2

        if fib_now[-1] == 0:
            fib_now.pop()
            if sum([fib[x] for x in fib_now]) > 1:
                target.append([sum([fib[x] for x in fib_now]), 2])
                target[-2][1] = 0

    target.reverse()
    # print(target)

    action = [0, 1]

    next = 0
    for x in target:
        if x[1] == 0:
            action.append(x[1] + next ^ 1)
        else:
            action.append(x[1] + next)
            next ^= 1
    if next == 0:
        for i, _ in enumerate(action):
            action[i] ^= 1

    print(len(action))
    for x in action:
        print(x+1)

    # x = 0
    # y = 0
    # for z in action:
    #     if z+1 == 1:
    #         x += 1
    #     if z+1 == 2:
    #         y += 1
    #     if z+1 == 3:
    #         x = x + y
    #     if z+1 == 4:
    #         y = x + y
    #     print(x, y)

def test(n):
    fib = [1, 1]
    while fib[-1] < 10 ** 18:
        fib.append(fib[-1] + fib[-2])

    x = n
    fib_now = []
    for i in range(len(fib)-1, -1, -1):
        if x >= fib[i]:
            fib_now.append(i)
            x -= fib[i]

    target = [[n, 2]]

    while fib_now:
        for i, _ in enumerate(fib_now):
            fib_now[i] -= 1

        if sum([fib[x] for x in fib_now]) > 1:
            target.append([sum([fib[x] for x in fib_now]), 2])
            target[-2][1] = 2

        if fib_now[-1] == 0:
            fib_now.pop()
            if sum([fib[x] for x in fib_now]) > 1:
                target.append([sum([fib[x] for x in fib_now]), 2])
                target[-2][1] = 0

    target.reverse()
    # print(target)

    action = [0, 1]

    next = 0
    for x in target:
        if x[1] == 0:
            action.append(x[1] + next ^ 1)
        else:
            action.append(x[1] + next)
            next ^= 1
    if next == 0:
        for i, _ in enumerate(action):
            action[i] ^= 1

    # print(len(action))
    # for x in action:
    #     print(x+1)

    x = 0
    y = 0
    for z in action:
        if z+1 == 1:
            x += 1
        if z+1 == 2:
            y += 1
        if z+1 == 3:
            x = x + y
        if z+1 == 4:
            y = x + y
        # print(x, y)
    assert(x == n)

    return len(action)


if __name__ == '__main__':
    # for i in range(1000):
    #     n = random.randint(1, 10 ** 18)
    #     res = test(n)
    #     print(n, res)
    #     if (res > 130):
    #         break
    main()
