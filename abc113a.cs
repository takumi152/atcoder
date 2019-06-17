using System;

public class ABC113A {
  static public void Main(){
    string[] buf = Console.ReadLine().Split(' ');
    int X = int.Parse(buf[0]);
    int Y = int.Parse(buf[1]);

    int fare = X + (Y / 2);
    Console.WriteLine(fare);
  }
}
