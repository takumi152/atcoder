using System;

public class ABC079C {
  static public void Main(){
    string[] buf;
    buf = Console.ReadLine().Split(' ');
    int n = int.Parse(buf[0]);

    int a = n / 1000;
    int b = (n / 100) % 10;
    int c = (n / 10) % 10;
    int d = n % 10;
    for (int i = 0; i < 8; i++) {
      int ans = a;
      if ((i & 4) > 0) ans += b;
      else ans -= b;
      if ((i & 2) > 0) ans += c;
      else ans -= c;
      if ((i & 1) > 0) ans += d;
      else ans -= d;
      if (ans == 7) {
        Console.Write(a);
        if ((i & 4) > 0) Console.Write('+');
        else Console.Write('-');
        Console.Write(b);
        if ((i & 2) > 0) Console.Write('+');
        else Console.Write('-');
        Console.Write(c);
        if ((i & 1) > 0) Console.Write('+');
        else Console.Write('-');
        Console.Write(d);
        Console.WriteLine("=7");
        break;
      }
    }
  }
}
