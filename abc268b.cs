using System;
using System.Collections;
using System.Collections.Generic;

public class ABC268B {
  static public void Main() {
    var s = Console.ReadLine();
    var t = Console.ReadLine();

    if (s.Length > t.Length) {
      Console.Out.WriteLine("No");
    }
    else if (s == t.Substring(0, s.Length)) {
      Console.Out.WriteLine("Yes");
    }
    else {
      Console.Out.WriteLine("No");
    }
  }
}
