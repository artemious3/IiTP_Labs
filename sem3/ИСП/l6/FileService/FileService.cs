using System.Text.Json;

namespace PadhaiskiFileServices;
public class FileService : IFileService<Employee>
{
    public IEnumerable<Employee>? ReadFile(string fname)
    {
        using (var fstream = File.OpenText(fname))
        {
            return JsonSerializer.Deserialize<List<Employee>>(fstream.ReadToEnd());
        }
    }

    public void SaveData(IEnumerable<Employee> data, string fname)
    {
        using (var fstream = File.CreateText(fname))
        {

            var json = JsonSerializer.Serialize(fname);
            fstream.Write(json);
        }
    }
}
