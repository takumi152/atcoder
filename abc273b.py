from decimal import ROUND_HALF_UP, Decimal

def main():
    x, k = map(int, input().split())
    
    ans = Decimal(x)
    for i in range(k+1):
        ans = ans.quantize(Decimal(f"1E{i}"), rounding=ROUND_HALF_UP)
    
    print(int(ans))

if __name__ == '__main__':
    main()
