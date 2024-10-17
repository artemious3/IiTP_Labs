record class Passenger
{
    public string Name;
    public string Surname;
    public Int32 TicketID;
    public bool IsAdult;

}

class MyCustomComparer : IComparer<Passenger>
{
    public int Compare(Passenger? p1, Passenger? p2)
    {
        return p1.Surname.CompareTo(p2.Surname);
    }
}