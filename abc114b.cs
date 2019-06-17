using System;

public class ABC114B {
  static public void Main(string[] args){
    string[] buf;
    buf = Console.ReadLine().Split(' ');
    string S = buf[0];
    int target = 753;
    int minDiff = int.MaxValue;
    for (int i = 0; i < S.Length - 2; i++) {
      int num = int.Parse(S.Substring(i, 3));
      int diff = Math.Abs(num - target);
      if (diff < minDiff) {
        minDiff = diff;
      }
    }
    Console.WriteLine(minDiff);
  }
}
