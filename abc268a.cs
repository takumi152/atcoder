using System;
using System.Collections;
using System.Collections.Generic;

public class ABC268A {
  static public void Main() {
    string[] buf;

    buf = Console.ReadLine().Split(' ');
    var a = new List<int>();
    for (int i = 0; i < 5; i++) a.Add(int.Parse(buf[i]));

    var a_uniq = new HashSet<int>();
    foreach (var x in a) {
      a_uniq.Add(x);
    }

    Console.Out.WriteLine(a_uniq.Count);
  }
}
