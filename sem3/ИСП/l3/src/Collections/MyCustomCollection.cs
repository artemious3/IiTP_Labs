using System.Collections;

class Node<T>
{
    public T? data { get; set; }
    public Node<T>? next { get; set; }

    public Node(T t)
    {
        data = t;
    }
    public Node()
    {
    }
}





class MyCustomCollection<T> : ICustomCollection<T>, IEnumerable<T> where T : IEquatable<T>
{


    public class Enumerator : IEnumerator<T>
    {

        public Enumerator(MyCustomCollection<T> col)
        {
            parent_collection = col;
            cursor = new Node<T>();
            cursor.next = parent_collection.first;
        }



        private readonly MyCustomCollection<T> parent_collection;
        private Node<T> cursor;

        public void Reset()
        {
            cursor = new Node<T>();
            cursor.next = parent_collection.first;
        }

        public bool MoveNext()
        {
            if (cursor.next == null)
            {
                return false;
            }
            cursor = cursor.next;
            return true;
        }

        public T Current
        {
            get
            {
                return cursor.data;
            }
            set
            {
                cursor.data = value;
            }
        }
        object IEnumerator.Current => Current;

        public void Dispose() { }
    }

    private Node<T>? first = null;


    public IEnumerator<T> GetEnumerator()
    {
        return new Enumerator(this);
    }

    IEnumerator IEnumerable.GetEnumerator()
    {
        return (IEnumerator)GetEnumerator();
    }


    public int Count { get; private set; } = 0;

    private Node<T> getNodeWithIndex(int index)
    {
        if (index < -Count || index >= Count)
        {
            throw new IndexOutOfRangeException();
        }

        if (index < 0)
        {
            index = index + Count;
        }

        var current_node = first;
        for (int i = 0; i < index; i++)
        {
            current_node = current_node.next;
        }

        return current_node;
    }


    public T this[int index]
    {

        get
        {
            var node = getNodeWithIndex(index);
            return node.data;
        }

        set
        {
            var node = getNodeWithIndex(index);
            node.data = value;
        }
    }

    public void Remove(T item)
    {

        Node<T> pre_tdl = null;
        var tdl = first;
        while (tdl != null && !tdl.data.Equals(item))
        {
            pre_tdl = tdl;
            tdl = tdl.next;
        }

        if(tdl == null){
            throw new KeyNotFoundException();
        } 
        
        if(pre_tdl == null){
            first = first.next;
        } else {
            pre_tdl.next = tdl.next;
        }
        Count--;
    }


public void Add(T item)
{
    var new_node = new Node<T>(item);
    if (first == null)
    {
        first = new_node;
    }
    else
    {
        var previous_first = first;
        first = new Node<T>(item);
        first.next = previous_first;
    }
    Count++;
}


}
