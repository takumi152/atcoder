import random

def main():
    print(60, 15)
    for i in range(60):
        x = random.randint(-1000, 1000)
        y = random.randint(-1000, 1000)
        c = random.randint(1, 100)
        print(x, y, c)

if __name__ == '__main__':
    main()
