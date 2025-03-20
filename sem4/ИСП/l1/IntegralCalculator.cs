

class IntegralCalculator{

	public delegate void TickHandler(long ticks, int thread_id);

	public IntegralCalculator(int iterationsPerTick, int maxThreads){
		IterationsPerTick = iterationsPerTick;
		pool = new Semaphore(maxThreads, maxThreads);
	}

	public readonly int IterationsPerTick;

	private const double Step = 1e-8;
	private const double HalfStep = Step/2.0;
	private Semaphore pool;

	public double Evaluate(double beg, double end){
		pool.WaitOne();
		double accumulator = 0;
		int iterations = 0;
		long ticks = 0;
		for(double x = beg; x < end; x+=Step){
			accumulator += Math.Sin(x + HalfStep) * Step;
			iterations++;
			if(iterations == IterationsPerTick){
				ticks++;
				iterations = 0;
				Tick?.Invoke(ticks, System.Environment.CurrentManagedThreadId);
			}
		}
		pool.Release();
		return accumulator;
	}


	public event TickHandler? Tick;


}
