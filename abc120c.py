def main():
    s = input()

    ans = 0
    st = []
    for x in s:
        st.append(x)
        if len(st) >= 2:
            if st[-1] != st[-2]:
                st.pop()
                st.pop()
                ans += 2

    print(ans)

if __name__ == '__main__':
    main()
