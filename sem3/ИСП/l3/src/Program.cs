


Console.ForegroundColor = ConsoleColor.Green;
Console.WriteLine("JOURNALED HOUSING MAINTNANCE SYSTEM");
Console.WriteLine("by Artsiom Padhaiski from group 353501");
Console.WriteLine("______________________________________");
Console.ResetColor();

HousingMaintenance hm = new HousingMaintenance();
Journal hmjournal = new Journal();

(hm as HousingMaintenance).ServiceChanged +=
 (type, desc) => Console.WriteLine($"Program received event({type}) : {desc}"); ;
(hm as HousingMaintenance).ServiceOrderEvent += hmjournal.LogEvent;


hm.RegisterResident(new Person{id = 1, surname = "A"});
hm.RegisterResident(new Person{id = 2, surname = "B"});
hm.RegisterResident(new Person{id = 3, surname = "C"});
hm.RegisterResident(new Person{id = 4, surname = "D"});

hm.AddService(new Service
{
    Name = "s1",
    id = 1,
    Price = 30
});

hm.AddService(new Service
{
    Name = "s2",
    id = 2,
    Price = 70
});

hm.AddService(new Service
{
    Name = "s3",
    id = 3,
    Price = 300
});


hm.RegisterServicesConsumption("A", "s1");
hm.RegisterServicesConsumption("B", "s2");
hm.RegisterServicesConsumption("C", "s2");
hm.RegisterServicesConsumption("D", "s3");
hm.RegisterServicesConsumption("A", "s1");
hm.RegisterServicesConsumption("B", "s1");
hm.RegisterServicesConsumption("D", "s2");
hm.RegisterServicesConsumption("D", "s3");


Console.WriteLine($"Total consumption for service s1 : {hm.GetServiceConsumption("s1")}");
Console.WriteLine($"Total consumption for service s2 : {hm.GetServiceConsumption("s2")}");
Console.WriteLine($"Total consumption for service s3 : {hm.GetServiceConsumption("s3")}");

Console.WriteLine($"Resident A total fees : {hm.GetResidentServiceFees("A")}");
Console.WriteLine($"Resident B total fees : {hm.GetResidentServiceFees("B")}");
Console.WriteLine($"Resident C total fees : {hm.GetResidentServiceFees("C")}");
Console.WriteLine($"Resident D total fees : {hm.GetResidentServiceFees("D")}");

Console.WriteLine("_______________________________________________________\n");

Console.WriteLine("List of service fees\n");

hm.GetListOfServiceFees("A").ToList().ForEach(x => Console.WriteLine($"A: Service name: {x.Item1}; Fee {x.Item2}"));
hm.GetListOfServiceFees("B").ToList().ForEach(x => Console.WriteLine($"B: Service name: {x.Item1}; Fee {x.Item2}"));
hm.GetListOfServiceFees("C").ToList().ForEach(x => Console.WriteLine($"C: Service name: {x.Item1}; Fee {x.Item2}"));
hm.GetListOfServiceFees("D").ToList().ForEach(x => Console.WriteLine($"D: Service name: {x.Item1}; Fee {x.Item2}"));

Console.WriteLine("_______________________________________________________\n");

Console.WriteLine($"In total housing maintenance earned {hm.GetTotalFees()}");
Console.WriteLine($"Surname of resident with max fee is {hm.GetResidentWithMaxFee()}");
Console.WriteLine($"Number of residents with fee more than 200 is {hm.GetResidentsWithFeeMoreThan(200)}");

Console.WriteLine("_______________________________________________________\n");

Console.WriteLine("Journal contents:");
hmjournal.OutputAllEventsToConsole();

