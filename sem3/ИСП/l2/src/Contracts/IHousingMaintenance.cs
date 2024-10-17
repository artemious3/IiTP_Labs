

using HousingMaintenance;

interface IHousingMaintenance
{

    public void AddService(Service service);


    public void RegisterServicesConsumption(string surname, string service_name);

    public int GetServiceFees(string surname);

    public int GetServiceConsumption(string service_name);
    
    public int GetTotalConsumption();
    public int GetTotalFees();


}