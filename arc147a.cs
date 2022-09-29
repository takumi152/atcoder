using System;
using System.Collections;
using System.Collections.Generic;

public class PriorityQueue<T> : IEnumerable<T>
{
    private readonly List<T> _data = new List<T>();
    private readonly IComparer<T> _comparer;
    private readonly bool _isDescending;

    public PriorityQueue(IComparer<T> comparer, bool isDescending = true)
    {
        _comparer = comparer;
        _isDescending = isDescending;
    }

    public PriorityQueue(Comparison<T> comparison, bool isDescending = true)
        : this(Comparer<T>.Create(comparison), isDescending)
    {
    }

    public PriorityQueue(bool isDescending = true)
        : this(Comparer<T>.Default, isDescending)
    {
    }

    public void Enqueue(T item)
    {
        _data.Add(item);
        var childIndex = _data.Count - 1;
        while (childIndex > 0)
        {
            var parentIndex = (childIndex - 1) / 2;
            if (Compare(_data[childIndex], _data[parentIndex]) >= 0)
                break;
            Swap(childIndex, parentIndex);
            childIndex = parentIndex;
        }
    }

    public T Dequeue()
    {
        var lastIndex = _data.Count - 1;
        var firstItem = _data[0];
        _data[0] = _data[lastIndex];
        _data.RemoveAt(lastIndex--);
        var parentIndex = 0;
        while (true)
        {
            var childIndex = parentIndex * 2 + 1;
            if (childIndex > lastIndex)
                break;
            var rightChild = childIndex + 1;
            if (rightChild <= lastIndex && Compare(_data[rightChild], _data[childIndex]) < 0)
                childIndex = rightChild;
            if (Compare(_data[parentIndex], _data[childIndex]) <= 0)
                break;
            Swap(parentIndex, childIndex);
            parentIndex = childIndex;
        }
        return firstItem;
    }

    public T Peek()
    {
        return _data[0];
    }

    public int Size()
    {
        return _data.Count;
    }

    public bool Empty()
    {
        return this.Size() == 0;
    }

    private void Swap(int a, int b)
    {
        var tmp = _data[a];
        _data[a] = _data[b];
        _data[b] = tmp;
    }

    private int Compare(T a, T b)
    {
        return _isDescending ? _comparer.Compare(b, a) : _comparer.Compare(a, b);
    }

    public int Count => _data.Count;

    public IEnumerator<T> GetEnumerator()
    {
        return _data.GetEnumerator();
    }

    IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
}

public class ARC147A {
  static public void Main(){
    string[] buf;

    buf = Console.ReadLine().Split(' ');
    var n = int.Parse(buf[0]);

    buf = Console.ReadLine().Split(' ');
    var a = new List<int>();
    for (int i = 0; i < n; i++) a.Add(int.Parse(buf[i]));

    var smallestNum = a[0];
    for (int i = 0; i < n; i++) smallestNum = Math.Min(smallestNum, a[i]);

    var que = new PriorityQueue<int>();
    for (int i = 0; i < n; i++) que.Enqueue(a[i]);
    var ans = 0;

    while (que.Size() > 1) {
      var largestNum = que.Dequeue();
      var nextNum = largestNum % smallestNum;
      ans++;
      if (nextNum == 0) continue;
      que.Enqueue(nextNum);
      smallestNum = Math.Min(smallestNum, nextNum);
    }

    Console.Out.WriteLine(ans);
  }
}
