using System;
using System.Collections;
using System.Collections.Generic;

public static class ListExtensions
{
  public static void Swap<T>(this List<T> list, int index1, int index2)
  {
    T temp = list[index1];
    list[index1] = list[index2];
    list[index2] = temp;
  }
}

public class ARC147B {
  static void Swap<T>(ref T a, ref T b) {
    var t = a;
    a = b;
    b = t;
  }

  static public void Main() {
    string[] buf;

    buf = Console.ReadLine().Split(' ');
    var n = int.Parse(buf[0]);

    buf = Console.ReadLine().Split(' ');
    var p = new List<int>();
    for (int i = 0; i < n; i++) p.Add(int.Parse(buf[i]));

    var ans = new List<(char command, int idx)>();

    for (int i = n - 3; i >= 0; i--) {
      for (int j = i; j >= 0; j--) {
        if (p[j] % 2 == j % 2 && p[j] % 2 != p[j+2] % 2) {
          p.Swap(j, j+2);
          ans.Add(('B', j+1));
        }
      }
    }

    Console.Out.WriteLine(ans.Count);
    foreach (var x in ans) {
      Console.Out.WriteLine(x.command.ToString() + " " + x.idx.ToString());
    }
  }
}
