using System.Reflection;

const string fname = "FileService.dll";
const string serviceName = "PadhaiskiFileServices.FileService";
const string jsonFileName = "data.json";


Assembly asm = Assembly.LoadFrom(fname);
var type = asm.GetType(serviceName);
var fs = asm.CreateInstance(serviceName);
var saveMethod = type.GetMethod("SaveData");
var readMethod = type.GetMethod("ReadFile");


if(File.Exists(jsonFileName)){
    File.Delete(jsonFileName);
}

List<Employee> lst = new List<Employee>(){
    new Employee(){Id = 1, IsRemoteWorker=true, Name = "Ivan Ivanov"},
    new Employee(){Id = 2, IsRemoteWorker=true, Name = "Helen Bayers"},
    new Employee(){Id = 3, IsRemoteWorker=true, Name = "John Evon"},
    new Employee(){Id = 4, IsRemoteWorker=false,Name = "Mitchel Moorse"},
    new Employee(){Id = 5, IsRemoteWorker=true, Name = "Krause Mauenh"}
};

saveMethod.Invoke(fs, new object[]{lst, jsonFileName});


var deserializedLst = readMethod.Invoke(fs, new object[]{jsonFileName}) as IEnumerable<Employee>;

foreach( var p in deserializedLst )
{
    Console.WriteLine(p);
}



