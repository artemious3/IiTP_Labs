


class HousingMaintenanceImpl : IHousingMaintenance
{
    private MyCustomCollection<(string, CommunalServices)> data;
    public CommunalServices Tariff {get; set;}


    public HousingMaintenanceImpl()
    {
        data = new MyCustomCollection<(string, CommunalServices)>();
    }


    private int? FindIndexWithSurname(string surname)
    {
        int idx = -1;
        for(int i = 0; i < data.Count; i++)
        {
            if(data[i].Item1 == surname)
            {
                idx = i;
            }
        }

        if(idx == -1){
            return null;
        }
        return idx;
    }
   

    public void RegisterResident(string surname)
    {
        data.Add( (surname, new(0,0,0)) );
    }

    public void RegisterServicesConsumption(string surname, CommunalServices resources)
    {
        var index = FindIndexWithSurname(surname).Value;

        var item = data[index];
        item.Item2 = item.Item2 + resources;
        data[index] = item;
    }


    public CommunalServices GetServiceConsumption(string surname){
        var index = FindIndexWithSurname(surname).Value;
        return data[index].Item2;
    }

    public int GetServiceFees(string surname)
    {
        var idx = FindIndexWithSurname(surname).Value;
        return data[idx].Item2 * Tariff;
    }

    public CommunalServices GetTotalConsumption()
    {
        CommunalServices accumulator = new(0,0,0);
        for(int i = 0; i < data.Count; i++ )
        {
            accumulator += data[i].Item2;
        }
        return accumulator;

    }

    public int GetTotalFees()
    {   
        return GetTotalConsumption() * Tariff;        
    }


}