
record class Event
{
    public string Type { get; set; }
    public string Description { get; set; }

    public override string ToString()
    {
        return $"HM Event ({Type}): {Description}";
    }
}
