import java.security.SecureRandom;
import java.util.Scanner;

public class TestCase {

    static final int MIN_SIZE = 30;
    static final int MAX_SIZE = 30;
    static final int MIN_T = 10000;
    static final int MAX_T = 10000;
    static final double MIN_RATIO_K = 0.5;
    static final double MAX_RATIO_K = 0.5;

    SecureRandom rnd;
    int H, W, K, T;
    int[] A, B, C, D;

    TestCase(long seed) throws Exception {
        rnd = SecureRandom.getInstance("SHA1PRNG");
        rnd.setSeed(seed);
        this.H = rnd.nextInt(MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;
        this.W = rnd.nextInt(MAX_SIZE - MIN_SIZE + 1) + MIN_SIZE;
        double ratioK = rnd.nextDouble() * (MAX_RATIO_K - MIN_RATIO_K) + MIN_RATIO_K;
        this.K = (int) Math.ceil(this.H * this.W * ratioK);
        this.T = rnd.nextInt(MAX_T - MIN_T + 1) + MIN_T;
        this.A = new int[K];
        this.B = new int[K];
        this.C = new int[K];
        this.D = new int[K];
        int[] pos = new int[H * W];
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                pos[i * W + j] = (i << 16) | j;
            }
        }
        shuffle(pos);
        for (int i = 0; i < K; i++) {
            this.A[i] = (pos[i] & 0xFFFF) + 1;
            this.B[i] = (pos[i] >> 16) + 1;
        }
        shuffle(pos);
        for (int i = 0; i < K; i++) {
            this.C[i] = (pos[i] & 0xFFFF) + 1;
            this.D[i] = (pos[i] >> 16) + 1;
        }
    }

    private void shuffle(int[] ar) {
        for (int i = 0; i < ar.length; i++) {
            int to = rnd.nextInt(ar.length - i) + i;
            int tmp = ar[i];
            ar[i] = ar[to];
            ar[to] = tmp;
        }
    }

    TestCase(Scanner sc) throws Exception {
        this.H = sc.nextInt();
        this.W = sc.nextInt();
        this.K = sc.nextInt();
        this.T = sc.nextInt();
        this.A = new int[K];
        this.B = new int[K];
        this.C = new int[K];
        this.D = new int[K];
        for (int i = 0; i < this.K; i++) {
            this.A[i] = sc.nextInt();
            this.B[i] = sc.nextInt();
            this.C[i] = sc.nextInt();
            this.D[i] = sc.nextInt();
        }
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();
        builder.append(this.H + " " + this.W + " " + this.K + " " + this.T + "\n");
        for (int i = 0; i < K; ++i) {
            builder.append(this.A[i] + " " + this.B[i] + " " + this.C[i] + " " + this.D[i] + "\n");
        }
        return builder.toString();
    }

}
