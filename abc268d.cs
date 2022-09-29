using System;
using System.Collections;
using System.Collections.Generic;

public class ABC268D {
  static public void Main() {
    string[] buf;

    buf = Console.ReadLine().Split(' ');
    var n = int.Parse(buf[0]);
    var m = int.Parse(buf[1]);

    var s = new List<string>(new string[n]);
    for (int i = 0; i < n; i++) s[i] = Console.ReadLine();
    
    var t = new HashSet<string>();
    for (int j = 0; j < m; j++) t.Add(Console.ReadLine());

    Func<string, int, (string, bool)> dfs = null;
    dfs = (currentName, flag) => {
      for (int i = 0; i < n; i++) {
        if ((flag & (1 << i)) > 0) {
          if (currentName.Length + s[i].Length > 16) continue;
          var nextName = currentName + s[i];
          if ((flag & ~(1 << i)) == 0) {
            if (nextName.Length >= 3 && !t.Contains(nextName)) return (nextName, true);
            else return ("", false);
          }
          else {
            while (nextName.Length < 16) {
              nextName += "_";
              var (resultName, success) = dfs(nextName, flag & ~(1 << i));
              if (success) return (resultName, success);
            }
          }
        }
      }
      return ("", false);
    };

    var (finalResultName, finalSuccess) = dfs("", (1 << n) - 1);

    if (finalSuccess) Console.Out.WriteLine(finalResultName);
    else Console.Out.WriteLine(-1);
  }
}
