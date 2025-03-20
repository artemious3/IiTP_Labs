using System.Text.Json;
namespace L2.Lib;

public class StreamService<T>
{

	public async Task WriteToStreamAsync(Stream stream, IEnumerable<T> data, IProgress<(int, string)> progress)
	{
		Thread.Sleep(200);
		progress.Report((System.Environment.CurrentManagedThreadId, "ЗАПИСЬ В STREAM : Начало выполнения"));
		await JsonSerializer.SerializeAsync(stream, data);
		progress.Report((System.Environment.CurrentManagedThreadId, "ЗАПИСЬ В STREAM : Конец выполнения"));

	}
	public async Task CopyFromStreamAsync(Stream stream, string filename, IProgress<(int, string)> progress)
	{
		progress.Report((System.Environment.CurrentManagedThreadId, "КОПИЯ ИЗ STREAM : Начало выполнения"));
		using(var fs = File.Open(filename, FileMode.Create))
		{
			await stream.CopyToAsync(fs);
		}
		progress.Report((System.Environment.CurrentManagedThreadId, "КОПИЯ ИЗ STREAM : Конец выполнения"));
	}

	public async Task<int> GetStatisticsAsync(string fileName, Func<T, bool> filter)
	{
		using(var fs = File.OpenRead(fileName))
		{
			var collection = await JsonSerializer.DeserializeAsync< IEnumerable<T> >(fs);
			var counter = 0;
			foreach(T el in collection)
			{
				if(filter(el)){
					counter++;
				}
			}
			return counter;
		}
	}
}
