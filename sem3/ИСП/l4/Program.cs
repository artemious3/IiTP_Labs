using System.Security.AccessControl;
using System.Security.Cryptography.X509Certificates;

const string dirName = "Подгайский_Lab4";
var extensions = new string[] { ".txt", ".rtf", ".dat", ".inf" };

const string name1 = "file1.bin";
const string name2 = "file2.bin";

Random rand = new Random();


Console.WriteLine("________________________________");


var dirInfo = new DirectoryInfo(dirName);

if (dirInfo.Exists)
{
    Console.WriteLine($"Directory {dirName} already exists. Removing...");
    var files = dirInfo.GetFiles();
    foreach (var file in files)
    {
        file.Delete();
    }
    Console.WriteLine($"Removed {files.Length} files");
    dirInfo.Delete();
}

dirInfo.Create();


for (int i = 0; i < 10; i++)
{
    string newFileName = Path.GetRandomFileName() + extensions[rand.Next(extensions.Length)];
    File.Create(dirInfo.Name + '/' + newFileName);
}

foreach (var file in dirInfo.GetFiles())
{
    var name_and_extention = file.Name.Split('.');
    Console.WriteLine($"Файл {file.Name} имеет расширение {name_and_extention.Last()}");
}


Console.WriteLine("________________________________");

var sourcePassengerList = new List<Passenger>{
    new Passenger(){Name = "John", Surname="Doe", TicketID=374623, IsAdult = true },
    new Passenger(){Name = "Helena", Surname="Jackson", TicketID=184692, IsAdult = true },
    new Passenger(){Name = "Alan", Surname="Simpson", TicketID=278883, IsAdult = false },
    new Passenger(){Name = "Elon", Surname="Peterson", TicketID=119923, IsAdult = true },
    new Passenger(){Name = "Selena", Surname="Musk", TicketID=912723, IsAdult = false }
};

if (File.Exists(name1))
{
    File.Delete(name1);
}

Console.WriteLine($"Writing passenger list to {name1}");
PasengerFileService pfs = new PasengerFileService();

try
{
    pfs.SaveData(sourcePassengerList, name1);
}
catch (IOException ex)
{
    Console.WriteLine($"Could not read data: {ex.Message}");
}


if (File.Exists(name2))
{
    File.Delete(name2);
}

File.Move(name1, name2);

Console.WriteLine($"Renamed passenger list to {name2} and read data");


var passengerList = pfs.ReadFile(name2);

var sortedPassengerList = sourcePassengerList.Order(new MyCustomComparer()).ToList();
var sortedByIdPassengerList = passengerList.OrderBy(x => x.TicketID);

Console.WriteLine("Not sorted passenger list:");
passengerList.ToList().ForEach(x => Console.WriteLine(x));

Console.WriteLine("\nSorted passenger list (by name)");
sortedPassengerList.ToList().ForEach(x => Console.WriteLine(x));

Console.WriteLine("\nSorted passenger list (by ticket id)");
sortedByIdPassengerList.ToList().ForEach(x => Console.WriteLine(x));










