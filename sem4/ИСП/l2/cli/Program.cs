using System.Text.Json;
using L2.Lib;
class Program
{

	static async Task Main(string[] args)
	{
		Random rnd = new Random();
		List<Passenger> lst = new List<Passenger>();
		for(int i = 0; i < 1000; ++i){
			lst.Add(new Passenger{
					Id = (uint)rnd.NextInt64(),
					Name = "John Doe",
					HasLuggage = rnd.Next(0,10) != 5 
					});
		}

		Console.WriteLine($"ID потока Main : {System.Environment.CurrentManagedThreadId}");


		MemoryStream ms = new MemoryStream();
		Progress<(int, string)> progress = new Progress<(int, string)>();
		progress.ProgressChanged += (_, msg)=>
		{
			Console.WriteLine($"ПОТОК {msg.Item1} : {msg.Item2}");
		};


		StreamService<Passenger> ss = new StreamService<Passenger>();
		var writeTask = Task.Run(() => ss.WriteToStreamAsync(ms, lst, progress) ) ;
		Thread.Sleep(200);
		ms.Seek(0, SeekOrigin.Begin);

		var copyTask = ss.CopyFromStreamAsync(ms, "passengers.json", progress);

		await writeTask;
		await copyTask;


		var statistics = await ss.GetStatisticsAsync("passengers.json", (el) =>{
				return el.HasLuggage;
				});
		Console.WriteLine($"{statistics} человек имеет багаж");


	}
}
