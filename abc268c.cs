using System;
using System.Collections;
using System.Collections.Generic;

public class ABC268C {
  static public void Main() {
    string[] buf;

    buf = Console.ReadLine().Split(' ');
    var n = int.Parse(buf[0]);

    buf = Console.ReadLine().Split(' ');
    var p = new List<int>();
    for (int i = 0; i < n; i++) p.Add(int.Parse(buf[i]));

    for (int i = 0; i < n; i++) p[i] = (p[i] + n - i) % n;

    var bucket = new List<int>(new int[n]);
    for (int i = 0; i < n; i++) bucket[p[i]]++;

    var ans = 0;
    for (int i = 0; i < n; i++) {
      var score = 0;
      for (int j = i; j < i + 3; j++) score += bucket[j % n];
      if (score > ans) ans = score;
    }

    Console.Out.WriteLine(ans);
  }
}
