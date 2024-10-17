
namespace HousingMaintenance
{


    class HousingMaintenanceImpl : IHousingMaintenance
    {
        private MyCustomCollection<Service> services;
        private MyCustomCollection<(string, uint)> service_consumption;
        uint max_id = 0;

        public event LogEvent ServiceOrderEvent;
        public event LogEvent ServiceChanged;


        public HousingMaintenanceImpl()
        {
            services = new MyCustomCollection<Service>();
            service_consumption = new MyCustomCollection<(string, uint)>();
        }

        private Service? GetServiceById(uint id)
        {
            foreach (var s in services)
            {
                if (s.id == id)
                {
                    return s;
                }
            }
            return null;
        }

        private Service? GetServiceByName(string name)
        {
            foreach (var s in services)
            {
                if (s.Name == name)
                {
                    return s;
                }
            }
            return null;
        }


        public void RegisterServicesConsumption(string surname, string service_name)
        {
            var service = GetServiceByName(service_name);
            if (service == null)
            {
                throw new KeyNotFoundException("Service not found");
            }

            service_consumption.Add((surname, service.id));
            ServiceOrderEvent("ServiceConsumption", $"Resident {surname} ordered service {service_name}");
        }

        public void AddService(Service serv)
        {
            services.Add(serv);
            ServiceChanged("AddService", $"Service {serv.Name} added. Price : {serv.Price}");
        }

        public int GetServiceConsumption(string service_name)
        {
            var serv = GetServiceByName(service_name);
            int count = 0;
            foreach (var consumption in service_consumption)
            {
                if (consumption.Item2 == serv.id)
                {
                    count++;
                }
            }
            return count;
        }

        public int GetServiceFees(string surname)
        {
            int fee = 0;
            foreach (var sc in service_consumption)
            {
                if (sc.Item1 == surname)
                {
                    var serv = GetServiceById(sc.Item2);
                    fee += serv.Price;
                }
            }
            return fee;
        }

        public int GetTotalConsumption()
        {
            return service_consumption.Count;
        }

        public int GetTotalFees()
        {
            int fee = 0;
            foreach (var sc in service_consumption)
            {
                var service = GetServiceById(sc.Item2);
                fee += service.Price;
            }
            return fee;
        }

    }

}