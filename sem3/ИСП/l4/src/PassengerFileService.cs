

class PasengerFileService : IFileService<Passenger>
{
    public IEnumerable<Passenger> ReadFile(string fileName)
    {
        using (var fstream = File.Open(fileName, FileMode.Open, FileAccess.Read))
        {
            var reader = new BinaryReader(fstream);
            while(reader.PeekChar() > -1)
            {
                Passenger p = new Passenger();
                p.Name = reader.ReadString();
                p.Surname = reader.ReadString();
                p.TicketID = reader.ReadInt32();
                p.IsAdult = reader.ReadBoolean();
                yield return p;
            }
            yield break;
        }

    }
    public void SaveData(IEnumerable<Passenger> data, string fileName)
    {
        using (var fstream = File.Create(fileName))
        {

            var writer = new BinaryWriter(fstream);
            foreach (var p in data)
            {
                writer.Write(p.Name);
                writer.Write(p.Surname);
                writer.Write(p.TicketID);
                writer.Write(p.IsAdult);
            }
        }
    }
}