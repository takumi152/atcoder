
def main():
    buf = input()
    S = buf
    for i in range(3):
        if S[i] == S[i+1]:
            print('Bad')
            return
    print('Good')

if __name__ == '__main__':
    main()
