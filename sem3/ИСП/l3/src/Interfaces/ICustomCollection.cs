interface ICustomCollection<T> {

    T this[int index]{get; set;}
    int Count { get; }

    void Add(T item);
    void Remove(T item);
}