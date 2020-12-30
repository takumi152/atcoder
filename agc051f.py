# WIP

from decimal import Decimal, ROUND_UP, ROUND_DOWN

def main():
    q = int(input())
    query = [None for _ in range(q)]
    for i in range(q):
        query[i] = tuple(map(int, input().split()))

    y = [1, 3, 5, 17, 29, 99, 169, 577, 985, 3363, 5741, 19601, 33461, 114243, 195025, 665857, 1136689, 3880899, 6625109, 22619537, 38613965, 131836323, 225058681, 768398401]
    z = [1, 2, 7, 12, 41, 70, 239, 408, 1393, 2378, 8119, 13860, 47321, 80782, 275807, 470832, 1607521, 2744210, 9369319, 15994428, 54608393, 93222358, 318281039, 543339720]

    ans = [None for _ in range(q)]
    for i in range(q):
        if query[i][0] >= 0 and query[i][1] >= 0:
            ans[i] = 'Yes'
        elif query[i][0] < 0:
            good = False
            for x in y:
                k = (Decimal(abs(query[i][0])) / (Decimal(x) * Decimal(2).sqrt()).quantize(Decimal(1), rounding = ROUND_DOWN)).quantize(Decimal(1), rounding = ROUND_UP)
                print(x, Decimal(abs(query[i][0])), (Decimal(x) * Decimal(2).sqrt()), Decimal(abs(query[i][0])) / (Decimal(x) * Decimal(2).sqrt()).quantize(Decimal(1), rounding = ROUND_DOWN), k, (k + 1) * x)
                if (k + 1) * x <= query[i][1]:
                    good = True
                    break
            if good:
                ans[i] = 'Yes'
            else:
                ans[i] = 'No'
        elif query[i][1] < 0:
            good = False
            for x in z:
                k = (Decimal(abs(query[i][1])) / x).quantize(Decimal(1), rounding = ROUND_UP)
                print(x, k)
                if (k + 1) * (Decimal(query[i][1]) * Decimal(2).sqrt()).quantize(Decimal(1), rounding = ROUND_UP) >= query[i][0]:
                    good = True
                    break
            if good:
                ans[i] = 'Yes'
            else:
                ans[i] = 'No'

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
