namespace Padhaiski.Domain;
public interface ISerializer<T>
{
    IEnumerable<T> DeSerializeByLINQ(string fileName);
    IEnumerable<T> DeSerializeXML(string fileName);
    IEnumerable<T> DeSerializeJSON(string fileName);
    void SerializeByLINQ(IEnumerable<T> xxx, string fileName);
    void SerializeXML(IEnumerable<T> xxx, string fileName);
    void SerializeJSON(IEnumerable<T> xxx, string fileName);
}