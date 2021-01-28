import math

def main():
    a, b, h, m = map(int, input().split())

    a_deg = (30 * h + 0.5 * m) * (math.pi / 180)
    b_deg = (6 * m) * (math.pi / 180)

    ax = a * math.sin(a_deg)
    ay = a * math.cos(a_deg)
    bx = b * math.sin(b_deg)
    by = b * math.cos(b_deg)

    print(((ax - bx) ** 2 + (ay - by) ** 2) ** 0.5)

if __name__ == '__main__':
    main()
