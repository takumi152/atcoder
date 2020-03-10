using System;

public class keyence2019b {
  static public void Main(){
    string s = Console.ReadLine();

    int n = s.Length;

    string keyence = "keyence";
    for (int i = 0; i <= 7; i++) {
      if (s.Substring(0, i) == keyence.Substring(0, i) && s.Substring(n-7+i, 7-i) == keyence.Substring(i, 7-i)) {
        Console.WriteLine("YES");
        return;
      }
    }

    Console.WriteLine("NO");

    return;
  }
}
