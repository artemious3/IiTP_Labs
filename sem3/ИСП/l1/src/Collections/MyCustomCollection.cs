

class Node<T>
{
    public T data { get; set; }
    public Node<T>? next { get; set; }

    public Node(T t)
    {
        data = t;
    }
}



class MyCustomCollection<T> : ICustomCollection<T> where T : IEquatable<T>
{
    private Node<T>? first = null;
    private Node<T>? last = null;
     private Node<T>? cursor = null;

    public int Count { get; private set; } = 0;
   

    private Node<T> getNodeWithIndex(int index)
    {
        if (index < -Count && index >= Count)
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

    private void UpdateLast()
    {

        if (first == null)
        {
            last = null;
            return;
        }

        var pivot = first;
        while (pivot.next != null)
        {
            pivot = pivot.next;
        }

        last = pivot;
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

    public void Reset()
    {
        cursor = first;
    }

    public void Next()
    {
        cursor = cursor.next;
    }

    public T Current()
    {
        return cursor.data;
    }

    public void Remove(T item)
    {


        if (item.Equals(first.data))
        {
            first = first.next;
        }
        else
        {

            var pivot = first;
            while (!pivot.next.data.Equals(item))
            {
                pivot = pivot.next;
            }

            pivot.next = pivot.next.next;
        }

        UpdateLast();
        Count--;
    }


    public T RemoveCurrent()
    {
        var last_cursor = cursor;

        //search for next
        var pivot = first;
        while (pivot.next != cursor)
        {
            pivot = pivot.next;
        }

        pivot.next = cursor.next;
        Count--;
        UpdateLast();

        return last_cursor.data;
    }


    public void Add(T item)
    {
        var new_node = new Node<T>(item);
        if (first == null)
        {
            first = new_node;
            last = first;
        }
        else
        {
            last.next = new_node;
            last = last.next;
        }
        Count++;
    }
}
    