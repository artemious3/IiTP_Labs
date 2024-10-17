namespace SerializerLib;

using System.Xml.Linq;
using System.Xml.Serialization;
using Newtonsoft.Json;
using Padhaiski.Domain;

public class Serializer : ISerializer<Hospital>
{

    public void SerializeXML(IEnumerable<Hospital> data, string fileName)
    {
        var dataAsList = data.ToList();
        var xmlSerializer = new XmlSerializer(typeof(List<Hospital>));
        using (var fstream = File.Create(fileName))
        {
            xmlSerializer.Serialize(fstream, dataAsList);
        }
    }

    public IEnumerable<Hospital> DeSerializeXML(string fileName)
    {
        var xmlSerializer = new XmlSerializer(typeof(List<Hospital>));
        using (var fstream = File.Open(fileName, FileMode.Open, FileAccess.Read))
        {
            return (IEnumerable<Hospital>)xmlSerializer.Deserialize(fstream);
        }
    }

    public void SerializeJSON(IEnumerable<Hospital> data, string fileName)
    {
        using (var fstream = File.CreateText(fileName))
        {
            var json = JsonConvert.SerializeObject(data);
            fstream.Write(json);
        }
    }

    public IEnumerable<Hospital> DeSerializeJSON(string fileName)
    {
        return JsonConvert.DeserializeObject<IEnumerable<Hospital>>(File.ReadAllText(fileName));
    }

    public void SerializeByLINQ(IEnumerable<Hospital> data, string fileName)
    {
        XDocument doc = new XDocument();
        XElement root = new XElement("Hospitals");
        foreach(var er in data)
        {
            XElement erElement = new XElement("Hospital");
            foreach(var patient in er.WaitingList)
            {
                XElement xpatient = new XElement("Patient",
                new XElement("Name", patient.Name),
                new XElement("Surname", patient.Surname),
                new XElement("Age", patient.Age),
                new XElement("Diagnosis", patient.Diagnosis));
                erElement.Add(xpatient);
            }
             root.Add(erElement);
        }

        doc.Add(root);
        doc.Save(fileName);
    }

    public IEnumerable<Hospital> DeSerializeByLINQ(string fileName)
    {

        XDocument xdoc = XDocument.Load(fileName);
        var root = xdoc.Root;
        var emergencyRooms = root.Elements("Hospital").ToList();

        List<Hospital> lst = new List<Hospital>();

        foreach( var node in emergencyRooms ){
            Hospital er = new Hospital(); 
            foreach(var patient in node.Elements("Patient"))
            {
                Patient p = new Patient();
                p.Age = ushort.Parse(patient.Element("Age").Value);
                p.Diagnosis = patient.Element("Diagnosis").Value;
                p.Name = patient.Element("Name").Value;
                p.Surname = patient.Element("Surname").Value;
                er.WaitingList.Add(p);
            }
            lst.Add(er);
        }

        return lst;
    }

}
