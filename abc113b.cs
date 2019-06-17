using System;

public class ABC113B {
  static public void Main(){
    string[] buf;
    buf = Console.ReadLine().Split(' ');
    int N = int.Parse(buf[0]);
    buf = Console.ReadLine().Split(' ');
    int T = int.Parse(buf[0]);
    int A = int.Parse(buf[1]);
    buf = Console.ReadLine().Split(' ');
    int[] H = new int[buf.Length];
    for (int i = 0; i < buf.Length; i++) {
      H[i] = int.Parse(buf[i]);
    }
    double closestDiff = double.PositiveInfinity;
    int closestIdx = -1;
    for (int i = 0; i < H.Length; i++) {
      double diff = Math.Abs(A - (T - H[i] * 0.006));
      if (diff < closestDiff) {
        closestIdx = i;
        closestDiff = diff;
      }
    }
    Console.WriteLine(closestIdx + 1);
  }
}
