
def main():
    buf = input()
    s = buf
    characters = []
    for i in s:
        if not i in characters:
            characters.append(i)
    min_iter_count = 999
    for i in characters:
        iter_count = 0
        t = s
        while True:
            flag = False
            new_t = ""
            for j in range(len(t) - 1):
                if t[j] == i:
                    new_t += i
                elif t[j+1] == i:
                    new_t += i
                    flag = True
                else:
                    new_t += t[j]
                    flag = True
            if t[-1] != i:
                flag = True
            t = new_t
            if flag:
                iter_count += 1
            else:
                break
        if iter_count < min_iter_count:
            min_iter_count = iter_count
    print(min_iter_count)

if __name__ == '__main__':
    main()
