import random

def main():
    print(200000, 9973)
    s = []
    for i in range(200000):
        s.append(str(random.randint(0, 9)))
    print(''.join(s))

if __name__ == '__main__':
    main()
