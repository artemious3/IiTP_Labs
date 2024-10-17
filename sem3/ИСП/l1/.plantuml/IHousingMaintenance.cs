

interface IHousingMaintenance
{

    public CommunalServices Tariff {get; set; }

    public void RegisterResident(string surname);

    public void RegisterServicesConsumption(string surname, CommunalServices rc);

    public CommunalServices GetServiceConsumption(string surname);

    public int GetServiceFees(string surname);
    
    public CommunalServices GetTotalConsumption();

    public int GetTotalFees();


}