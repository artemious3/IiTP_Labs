

IHousingMaintenance hm = new HousingMaintenanceImpl();

hm.RegisterResident("A");
hm.RegisterResident("B");
hm.RegisterResident("C");
hm.RegisterResident("D");

hm.Tariff = new(100, 150, 150);

Console.WriteLine($"Tariff is {hm.Tariff}");

hm.RegisterServicesConsumption("A", new(10, 30, 30));
hm.RegisterServicesConsumption("B", new(20, 100, 0));
hm.RegisterServicesConsumption("C", new(0, 40, 20));
hm.RegisterServicesConsumption("D", new(30, 50, 10));

hm.RegisterServicesConsumption("B", new(10, 10, 10));
hm.RegisterServicesConsumption("D", new(40, 40, 30));


Console.WriteLine($"Total consumption for resident A : {hm.GetServiceConsumption("A")}");
Console.WriteLine($"Total consumption for resident B : {hm.GetServiceConsumption("B")}");
Console.WriteLine($"Total consumption for resident C : {hm.GetServiceConsumption("C")}");
Console.WriteLine($"Total consumption for resident D : {hm.GetServiceConsumption("D")}");

Console.WriteLine($"Total consumption : {hm.GetTotalConsumption()}");
Console.WriteLine($"Total fees: {hm.GetTotalFees()}");