using System;
using System.Threading;

namespace l3;


public static class Evaluator {
	const double BEG = 0.0;
	const double END = 1.0;
	const double STEP = 0.0001;

	public static double Evaluate( IProgress<int> iprogress, CancellationTokenSource cancelSrc){
		const double CHECK_EVERY = 0.01;

		double acc = 0.0;
		double before_check_step = 0.0;
		int progress = 0;


		for(double x = BEG; x < END; x += STEP){
			acc += STEP * System.Math.Sin(x);	
			before_check_step+= STEP;

			if(before_check_step >= CHECK_EVERY){
				before_check_step = 0.0;
				progress++;
				iprogress.Report(progress);
			}

			if(cancelSrc.Token.IsCancellationRequested){
				break;
			}
		}
		return acc;
	}
}
