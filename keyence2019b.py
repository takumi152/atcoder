
def main():
    buf = input()
    S = buf
    if S[0:len("keyence")] == "keyence" or S[len(S)-len("keyence"):len(S)] == "keyence":
        print("YES")
        return
    for i in range(len("keyence")):
        if S[0:i] == "keyence"[0:i] and S[len(S)-(len("keyence")-i):len(S)] == "keyence"[i:len("keyence")]:
            print("YES")
            return
    print("NO")

if __name__ == '__main__':
    main()
