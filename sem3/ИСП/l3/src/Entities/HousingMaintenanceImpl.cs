using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Security.Cryptography;
    class HousingMaintenance
    {
        private Dictionary<string, Service> services;
        private List<Person> residents;

        private Dictionary<uint, List<string>> ordered_services;

        public event LogEvent ServiceOrderEvent;
        public event LogEvent ServiceChanged;


        public HousingMaintenance()
        {
            services = new Dictionary<string, Service>();
            residents = new List<Person>();
            ordered_services = new Dictionary<uint, List<string>>();
        }
        private Service? GetServiceByName(string name)
        {
            return services[name];
        }

        public void RegisterResident(Person p)
        {
            residents.Add(p);
        }


        public void RegisterServicesConsumption(string surname, string service_name)
        {
            var person_id = (from r in residents
                             where r.surname == surname
                             select r.id).First();

            if (!ordered_services.ContainsKey(person_id))
            {
                ordered_services[person_id] = new List<string>();
            }
            ordered_services[person_id].Add(service_name);


            ServiceOrderEvent("ServiceConsumption", $"Resident {surname} ordered service {service_name}");
        }

        public void AddService(Service serv)
        {
            services[serv.Name] = serv;
            ServiceChanged("AddService", $"Service {serv.Name} added. Price : {serv.Price}");
        }

        public int GetServiceConsumption(string service_name)
        {
            var serv = GetServiceByName(service_name);
            int count = 0;
            foreach (var kv in ordered_services)
            {
                count += (from order in kv.Value
                          where order == service_name
                          select order).Count();
            }
            return count;
        }

        public IEnumerable<Service> GetOrderedListOfServices()
        {
            return (from s in services
                    orderby s.Value.Name
                    select s.Value);
        }

        public int GetResidentServiceFees(string surnname)
        {
            uint id = (from p in residents
                       where p.surname == surnname
                       select p.id).First();
            return GetServiceFeesById(id);
        }

        public int GetServiceFeesById(uint id)
        {
            return (from s in ordered_services[id]
                    select services[s].Price).Sum();
        }

        public int GetTotalFees()
        {
            int piv = 0;
            foreach (var p in residents)
            {
                piv += GetResidentServiceFees(p.surname);
            }
            return piv;
        }

        public string GetResidentWithMaxFee()
        {
            int max = 0;
            string max_surname = "";
            foreach (var s in residents)
            {
                var x = GetResidentServiceFees(s.surname);
                if (x > max)
                {
                    max = x;
                    max_surname = s.surname;
                }
            }
            return max_surname;
        }


        public int GetResidentsWithFeeMoreThan(int fee)
        {
            return ( from id in (
                from r in residents
                select r.id
            )
            select GetServiceFeesById(id)).Count( (x) => x > fee );
        }

        private int GetTotalResidentServiceFee(uint id, string service)
        {
            return (from order in ordered_services[id]
                   where order == service
                   select services[order].Price).Sum();
        }

        public IEnumerable<(string, int)> GetListOfServiceFees(string surname)
        {
            var id = (from r in residents where r.surname == surname select r.id).First();
            
            return  from s in services
                    select (s.Key, GetTotalResidentServiceFee(id,s.Key));

        }


    }
