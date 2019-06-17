
def main():
    buf = input()
    S = list(buf)
    A_count = 0
    B_appeared = False
    op_count = 0
    for pointer, _ in enumerate(S):
        if S[pointer] == 'A':
            if B_appeared:
                B_appeared = False
                A_count = 0
            A_count += 1
        elif S[pointer] == 'B':
            if A_count == 0:
                pass
            elif not B_appeared:
                B_appeared = True
            else: # B_appeared
                B_appeared = False
                A_count = 0
        else: # S[pointer] == 'C'
            if not B_appeared:
                A_count = 0
            else:
                op_count += A_count
                B_appeared = False
    print(op_count)

if __name__ == '__main__':
    main()
