namespace HousingMaintenance
{

    delegate void LogEvent(string type, string desc);
    class Journal
    {
        private List<Event> events;

        public Journal()
        {
            events = new List<Event>();
        }
        public void LogEvent(string type, string desc)
        {
            events.Add(new Event{
                Type = type,
                Description = desc
            });
        }

        public void OutputAllEventsToConsole()
        {
            foreach(var ev in events)
            {
                Console.WriteLine(ev);
            }
        }

    }
}