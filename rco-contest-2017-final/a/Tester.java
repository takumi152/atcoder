import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.Random;
import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.ForkJoinTask;
import java.util.concurrent.TimeUnit;

public class Tester {

    private static final int N = 8;
    private static final int MIN_CAPACITY = 1;
    private static final int MAX_CAPACITY = 10;
    private static final int MIN_DEMAND = 1;
    private static final int MAX_DEMAND = 50;
    private static final int MIN_TIME = 1;
    private static final int MAX_TIME = 10;
    private static final int PERIOD = 1000;

    /*
     * transported from testlib.h
     * https://github.com/MikeMirzayanov/testlib/blob/af0a6d35628f98129195bebea67d0ab8ca9f2598/testlib.h
     *
     * Copyright (c) 2005-2016 Mike Mirzayanov
     */
    static class TestlibRandom {
        private static final long multiplier = 0x5DEECE66DL;
        private static final long addend = 0xB;
        private static final long mask = (1L << 48) - 1;

        private long seed;

        void setSeed(long s) {
            seed = (s ^ multiplier) & mask;
        }

        long nextBits(int bits) {
            if (bits <= 48) {
                seed = (seed * multiplier + addend) & mask;
                return seed >> (48 - bits);
            } else {
                if (bits > 63) {
                    throw new RuntimeException("nextBits(int bits): n must be less than 64");
                }
                int lowerBitCount = 32;
                long left = (nextBits(31) << 32);
                long right = nextBits(lowerBitCount);
                return left ^ right;
            }
        }


        long next(long n) {
            if (n <= 0) {
                throw new RuntimeException("next(int n): n must be positive");
            }

            final long limit = Long.MAX_VALUE / n * n;
            long bits;
            do {
                bits = nextBits(63);
            } while (bits >= limit);

            return bits % n;
        }

        int next(int from, int to) {
            return (int) (next((long) to - from + 1) + from);
        }
    }

    static class State {
        TestlibRandom rnd;
        int[] C;
        int[] A;
        int D;
        int T;
        int score;

        State(long seed) throws Exception {
            rnd = new TestlibRandom();
            rnd.setSeed(seed);
            this.C = new int[N];
            this.A = new int[N];
            meetNewCustomer();
            for (int i = 0; i < N; i++) {
                this.C[i] = rnd.next(MIN_CAPACITY, MAX_CAPACITY);
            }
        }

        void fill(int i) {
            A[i] = C[i];
            advance();
        }

        void move(int from, int to) {
            if (from == to) {
                throw new RuntimeException("MOVE: same tank is specified as from and to.");
            }
            if (A[from] >= C[to] - A[to]) {
                A[from] -= C[to] - A[to];
                A[to] = C[to];
            } else {
                A[to] += A[from];
                A[from] = 0;
            }
            advance();
        }

        void change(int i) {
            A[i] = 0;
            C[i] = rnd.next(MIN_CAPACITY, MAX_CAPACITY);
            advance();
        }

        void pass() {
            T = 0;
            advance();
        }

        void sell(int[] idx) {
            int sum = 0;
            boolean[] used = new boolean[A.length];
            for (int i : idx) {
                if (A[i] == 0) {
                    throw new RuntimeException("SELL: empty tank is used.");
                }
                if (used[i]) {
                    throw new RuntimeException("SELL: tank " + (i + 1) + " is used more than once.");
                }
                sum += A[i];
                used[i] = true;
            }
            if (sum != D) {
                throw new RuntimeException("SELL: sum of the specified tanks " + sum + " does not equal to the D " + D + ".");
            }
            score += D * D;
            for (int i : idx) {
                A[i] = 0;
                C[i] = rnd.next(MIN_CAPACITY, MAX_CAPACITY);
            }
            T = 0; // hack
            advance();
        }

        private void advance() {
            --T;
            if (T <= 0) {
                meetNewCustomer();
            }
        }

        private void meetNewCustomer() {
            this.D = rnd.next(MIN_DEMAND, MAX_DEMAND);
            this.T = rnd.next(MIN_TIME, MAX_TIME);
        }


        @Override
        public String toString() {
            int N = C.length;
            StringBuilder builder = new StringBuilder();
            builder.append(D + " " + T + "\n");
            for (int i = 0; i < N; i++) {
                builder.append(C[i] + (i == N - 1 ? "\n" : " "));
            }
            for (int i = 0; i < N; i++) {
                builder.append(A[i] + (i == N - 1 ? "\n" : " "));
            }
            return builder.toString();
        }
    }

    static class Result {
        long seed;
        long elapsed;
        int score;

        @Override
        public String toString() {
            StringBuilder builder = new StringBuilder();
            builder.append("seed:" + seed + "\n");
            builder.append("elapsed:" + elapsed / 1000.0 + "\n");
            builder.append("score:" + score);
            return builder.toString();
        }
    }

    private void processQuery(State state, Scanner input) {
        String command = input.next();
        switch (command) {
            case "fill":
                int fi = input.nextInt() - 1;
                if (debug) {
                    System.out.println(command + " " + (fi + 1));
                }
                state.fill(fi);
                break;
            case "move":
                int from = input.nextInt() - 1;
                int to = input.nextInt() - 1;
                if (debug) {
                    System.out.println(command + " " + (from + 1) + " " + (to + 1));
                }
                state.move(from, to);
                break;
            case "change":
                int ci = input.nextInt() - 1;
                if (debug) {
                    System.out.println(command + " " + (ci + 1));
                }
                state.change(ci);
                break;
            case "pass":
                if (debug) {
                    System.out.println(command);
                }
                state.pass();
                break;
            case "sell":
                int n = input.nextInt();
                if (n <= 0 || N < n) {
                    throw new RuntimeException("SELL: number of tanks " + n + " is invalid.");
                }
                int[] si = new int[n];
                for (int i = 0; i < si.length; i++) {
                    si[i] = input.nextInt() - 1;
                }
                if (debug) {
                    System.out.print(command + " " + si.length);
                    for (int i = 0; i < si.length; i++) {
                        System.out.print(" " + (si[i] + 1));
                    }
                    System.out.println();
                }
                state.sell(si);
                break;
            default:
                throw new RuntimeException("unknown command " + command + ".");
        }
    }

    private Result execute(long seed) {
        Result res = new Result();
        res.seed = seed;
        try {
            State state = new State(seed);
            ProcessBuilder pb = new ProcessBuilder(command.split("\\s+"));
            Process proc = pb.start();
            OutputStream os = proc.getOutputStream();
            ForkJoinTask<?> future = ForkJoinPool.commonPool().submit(() -> {
                // redirect command stderr
                try (InputStreamReader reader = new InputStreamReader(proc.getErrorStream())) {
                    while (true) {
                        int ch = reader.read();
                        if (ch == -1) break;
                        System.err.print((char) ch);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            });
            long startTime = System.currentTimeMillis();
            try (Scanner sc = new Scanner(proc.getInputStream())) {
                for (int i = 0; i < PERIOD; i++) {
                    if (debug) {
                        System.out.println("time:" + i);
                        System.out.print(state);
                    }
                    os.write(state.toString().getBytes());
                    os.flush();
                    processQuery(state, sc);
                }
                res.elapsed = System.currentTimeMillis() - startTime;
                res.score = state.score;
                future.get(10, TimeUnit.SECONDS); // wait termination
            } finally {
                proc.destroy();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return res;
    }

    private static String command;
    private static boolean debug;

    public static void main(String[] args) {
        long seed = new Random().nextInt();
        for (int i = 0; i < args.length; ++i) {
            if (args[i].equals("-seed")) {
                seed = Long.parseLong(args[++i]);
            } else if (args[i].equals("-command")) {
                command = args[++i];
            } else if (args[i].equals("-debug")) {
                debug = true;
            }
        }
        if (command == null) {
            System.err.println("usage: java Tester -command command [-seed seed] [-debug]");
            System.exit(1);
        }
        Tester tester = new Tester();
        Result res = tester.execute(seed);
        System.out.println(res);
    }

}
