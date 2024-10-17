using Padhaiski.Domain;
using SerializerLib;
using Microsoft.Extensions.Configuration;
using System.Net.NetworkInformation;


void OutputIEnumerable<T>(IEnumerable<T> ienum)
{
    ienum.ToList().ForEach(x => Console.WriteLine(x));
}

bool CompareIEnumerable<T>(IEnumerable<T> A, IEnumerable<T> B) where T : IEquatable<T>
{
    if (A.Count() != B.Count())
        return false;

    for (int i = 0; i < A.Count(); ++i)
    {
        if (!A.ElementAt(i).Equals(B.ElementAt(i)))
        {
            return false;
        }
    }
    return true;
}



IConfiguration configuration = new ConfigurationBuilder()
    .SetBasePath(Directory.GetCurrentDirectory())
    .AddJsonFile("appsettings.json")
    .Build();

string xmlFile = configuration.GetSection("xmlFile").Value.ToString();
string jsonFile = configuration.GetSection("jsonFile").Value.ToString();
string xmlLinqFile = configuration.GetSection("xmlToLinqFile").Value.ToString();


Hospital ER = new Hospital();
ER.WaitingList.AddRange(new List<Patient> {
    new Patient{Name = "John", Surname="Doe", Age = 42, Diagnosis="Pneumonia"},
    new Patient{Name = "Alan", Surname="Simpson", Age=33, Diagnosis="Nausea" },
    new Patient{Name = "Helena", Surname="Jackson", Age=23, Diagnosis="Broken leg"},
    new Patient{Name = "Elon", Surname="Peterson", Age=78, Diagnosis="Flu" },
    new Patient{Name = "Selena", Surname="Musk", Age=65, Diagnosis="Broken arm"}
 }
);

List<Hospital> ERList = new List<Hospital>();
for (int i = 0; i < 5; ++i)
{
    ERList.Add(ER);
}


if (File.Exists(xmlFile)) { File.Delete(xmlFile); }
if (File.Exists(jsonFile)) { File.Delete(jsonFile); }
if (File.Exists(xmlLinqFile)) { File.Delete(xmlLinqFile); }


var serializer = new Serializer();
serializer.SerializeJSON(ERList, jsonFile);
serializer.SerializeXML(ERList, xmlFile);
serializer.SerializeByLINQ(ERList, xmlLinqFile);

Console.WriteLine("JSON FILE");
var jsonDeserialized = serializer.DeSerializeJSON(jsonFile);
OutputIEnumerable(jsonDeserialized);
Console.WriteLine($"Is equal to source collection: {CompareIEnumerable(ERList, jsonDeserialized)}\n");

Console.WriteLine("XML FILE");
var xmlDeserialized = serializer.DeSerializeXML(xmlFile);
OutputIEnumerable(xmlDeserialized);
Console.WriteLine($"Is equal to source collection: {CompareIEnumerable(ERList, xmlDeserialized)}\n");


Console.WriteLine("LINQ-JSON FILE");
var linqDeserialized = serializer.DeSerializeByLINQ(xmlLinqFile);
OutputIEnumerable(linqDeserialized);
Console.WriteLine($"Is equal to source collection: {CompareIEnumerable(ERList, linqDeserialized)}\n");





