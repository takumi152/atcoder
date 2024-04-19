import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.Scanner;

public class Judge {

    static final int[] DR = {-1, 0, 1, 0};
    static final int[] DC = {0, 1, 0, -1};

    static class Result {
        TestCase input;
        double score;
        double penaTime;
        double penaDist;

        @Override
        public String toString() {
            StringBuilder builder = new StringBuilder();
            builder.append("P_D:" + penaDist + "\n");
            builder.append("P_T:" + penaTime + "\n");
            builder.append("score:" + (int)Math.ceil(score) + "\n");
            return builder.toString();
        }
    }

    static class Output {
        char[][] move;

        Output(Scanner sc) {
            int C = sc.nextInt();
            this.move = new char[C][];
            for (int i = 0; i < C; i++) {
                this.move[i] = sc.next().toCharArray();
            }
        }
    }

    static Result calcScore(TestCase testcase, Output output) {
        final int EMPTY = -1;
        final int RESERVED = -2;
        Result res = new Result();
        res.input = testcase;
        int L = output.move.length;
        if (L > testcase.T) {
            throw new RuntimeException("move count " + L + " is larger than T.");
        }
        int[][] car = new int[testcase.H][testcase.W];
        for (int[] a : car) {
            Arrays.fill(a, EMPTY);
        }
        for (int i = 0; i < testcase.K; i++) {
            int r = testcase.A[i] - 1;
            int c = testcase.B[i] - 1;
            car[r][c] = i;
        }
        for (int i = 0; i < L; i++) {
            if (output.move[i].length != testcase.K) {
                throw new RuntimeException("command length does not equal to K at time " + i + ".");
            }
            for (int r = 0; r < testcase.H; r++) {
                for (int c = 0; c < testcase.W; c++) {
                    if (car[r][c] == EMPTY || car[r][c] == RESERVED) continue;
                    char command = output.move[i][car[r][c]];
                    if (command == '-') continue;
                    int dir = "URDL".indexOf(command);
                    if (dir < 0) {
                        throw new RuntimeException("command " + command + " is not valid at time " + i + " car " + (car[r][c] + 1) + ".");
                    }
                    int nr = r + DR[dir];
                    int nc = c + DC[dir];
                    if (nr < 0 || testcase.H <= nr || nc < 0 || testcase.W <= nc) {
                        throw new RuntimeException("invalid direction " + command + " at time " + i + " car " + (car[r][c] + 1) + ".");
                    }
                    if (car[nr][nc] == EMPTY) {
                        car[nr][nc] = RESERVED;
                    } else if (car[nr][nc] == RESERVED) {
                        throw new RuntimeException("multiple cars are moving to the same position at time " + i + ".");
                    } else {
                        throw new RuntimeException("car " + (car[r][c] + 1) + " is moving to the position another car exists at time " + i + ".");
                    }
                }
            }
            boolean[] moved = new boolean[testcase.K];
            for (int r = 0; r < testcase.H; r++) {
                for (int c = 0; c < testcase.W; c++) {
                    if (car[r][c] == EMPTY || car[r][c] == RESERVED) continue;
                    int carId = car[r][c];
                    if (moved[carId]) continue;
                    char command = output.move[i][carId];
                    if (command == '-') continue;
                    int dir = "URDL".indexOf(command);
                    int nr = r + DR[dir];
                    int nc = c + DC[dir];
                    car[nr][nc] = carId;
                    car[r][c] = EMPTY;
                    moved[carId] = true;
                }
            }
        }
        res.penaDist = 20;
        for (int r = 0; r < testcase.H; r++) {
            for (int c = 0; c < testcase.W; c++) {
                if (car[r][c] == EMPTY) continue;
                int carIdx = car[r][c];
                int dstR = testcase.C[carIdx] - 1;
                int dstC = testcase.D[carIdx] - 1;
                int dist = Math.abs(dstC - c) + Math.abs(dstR - r);
                res.penaDist += dist;
            }
        }
        res.penaTime = 10 + 0.01 * output.move.length;
        res.score = 10_000_000.0 / (res.penaDist * res.penaTime);
        return res;
    }

    public static void main(String[] args) throws Exception {
        if (args.length < 2) {
            System.err.println("usage: java Judge input_file_path output_file_path");
            System.exit(1);
        }
        Path inputFile = Paths.get(args[0]);
        Path outputFile = Paths.get(args[1]);
        TestCase testcase = new TestCase(new Scanner(inputFile));
        Output output = new Output(new Scanner(outputFile));
        Result res = calcScore(testcase, output);
        System.out.print(res);
    }

}
