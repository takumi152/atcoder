import random

def main():
    a = []
    for i in range(200000):
        a.append(random.randint(1, 9))
    print(''.join([str(x) for x in a]))

if __name__ == '__main__':
    main()
