


using HousingMaintenance;

class Program
{

    public static void Main()
    {
        Console.ForegroundColor = ConsoleColor.Green;
        Console.WriteLine("JOURNALED HOUSING MAINTNANCE SYSTEM");
        Console.WriteLine("by Artsiom Padhaiski from group 353501");
        Console.WriteLine("______________________________________");
        Console.ResetColor();

        IHousingMaintenance hm = new HousingMaintenance.HousingMaintenanceImpl();
        Journal hmjournal = new Journal();

        (hm as HousingMaintenanceImpl).ServiceChanged +=
         (type, desc) => Console.WriteLine($"Program received event({type}) : {desc}"); ;
        (hm as HousingMaintenanceImpl).ServiceOrderEvent += hmjournal.LogEvent;


        hm.AddService(new Service
        {
            Name = "s1",
            id = 1,
            Price = 30
        });

        hm.AddService(new HousingMaintenance.Service
        {
            Name = "s2",
            id = 2,
            Price = 70
        });

        hm.AddService(new HousingMaintenance.Service
        {
            Name = "s3",
            id = 3,
            Price = 300
        });

        hm.RegisterServicesConsumption("A", "s1");
        hm.RegisterServicesConsumption("B", "s2");
        hm.RegisterServicesConsumption("C", "s2");
        hm.RegisterServicesConsumption("D", "s3");

        hm.RegisterServicesConsumption("B", "s1");
        hm.RegisterServicesConsumption("D", "s2");


        Console.WriteLine($"Total consumption for service s1 : {hm.GetServiceConsumption("s1")}");
        Console.WriteLine($"Total consumption for service s2 : {hm.GetServiceConsumption("s2")}");
        Console.WriteLine($"Total consumption for service s3 : {hm.GetServiceConsumption("s3")}");

        Console.WriteLine($"Resident A fees : {hm.GetServiceFees("A")}");
        Console.WriteLine($"Resident B fees : {hm.GetServiceFees("B")}");
        Console.WriteLine($"Resident C fees : {hm.GetServiceFees("C")}");
        Console.WriteLine($"In TOTAL services ordered {hm.GetTotalConsumption()} times");
        Console.WriteLine($"Total resident fees: {hm.GetTotalFees()}");


        Console.WriteLine("Journal contents:");
        hmjournal.OutputAllEventsToConsole();





        Console.ForegroundColor = ConsoleColor.Green;
        Console.WriteLine("MyCustomCollections throwing exceptions");
        Console.ResetColor();


        MyCustomCollection<int> col = new MyCustomCollection<int>();
        try
        {
            Console.WriteLine(col[0]);
        }
        catch (IndexOutOfRangeException ex)
        {
            Console.WriteLine("index out of range exceprion thrown");
        }


        col.Add(4);
        col.Add(5);
        try
        {
            col.Remove(3);
        }
        catch (KeyNotFoundException ex)
        {
            Console.WriteLine("key not found exceprion thrown");
        }




    }
}
