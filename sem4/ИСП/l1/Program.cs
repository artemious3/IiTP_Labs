// See https://aka.ms/new-console-template for more information
using System.Diagnostics;

void OnTick(long ticks, int thread_id){
        string progress = new string('=', (int)ticks);
	Console.WriteLine($"ПОТОК {System.Environment.CurrentManagedThreadId} : [{progress.PadRight(10)}] {ticks*10}%");
}

void ManageThread(IntegralCalculator ic){
	Stopwatch sw = new Stopwatch();
	sw.Start();
	Console.WriteLine($"ПОТОК {System.Environment.CurrentManagedThreadId} :" +
				$" завершен с результатом {ic.Evaluate(0.0,1.0)}. Тиков : {sw.Elapsed.Ticks}");

}


void RunNThreads(int N, int maxSimultaneously){
	IntegralCalculator ic = new IntegralCalculator(10_000_000, maxSimultaneously);
	ic.Tick+=OnTick;


	List<Thread> threads = new List<Thread>();
	for(int i = 0; i < N; ++i){
		Thread t = new Thread(() => ManageThread(ic));
		t.Start();
		threads.Add(t);
	}
	foreach( var thr in threads){
		thr.Join();
	}
	
}


Console.Write("--- Integral Calculator ---\n\n\n");


Console.WriteLine("   PART 1 : simple evaluation\n\n");
{
	RunNThreads(1,1);
}

Console.WriteLine("\n   PART 2 : two threads with different priority\n\n");
{
	IntegralCalculator ic = new IntegralCalculator(10_000_000, 2);
	ic.Tick+=OnTick;
	Thread t1 = new Thread(() => ManageThread(ic));
	t1.Priority = ThreadPriority.Lowest;
	Thread t2 = new Thread(() => ManageThread(ic));
	t2.Priority = ThreadPriority.Highest;
	t1.Start();
	t2.Start();

	t1.Join();
	t2.Join();
}

Console.WriteLine("\n   PART 3 : 5 threads\n\n");
{
	RunNThreads(5,3);
}
