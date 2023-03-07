package doubleVectorElements;
/**
 * @author cristian.chilipirea
 *
 */
public class Main {

	public static void main(String[] args) {
		int N = 100000013;
		int v[] = new int[N];
		int NUM_THREADS = 4;
		int start;
		int stop;
		Thread[] t = new Thread[NUM_THREADS];
		
		for(int i=0;i<N;i++)
			v[i]=i;
		
		// Parallelize me
		for (int i = 0; i < NUM_THREADS; i++) {
			start = (int)(i * (double)N/NUM_THREADS);
			stop = (int)Math.min((i+1)*(double)N/NUM_THREADS, N);
			t[i] = new Task(start, stop, v);
			t[i].start();
		}
		for (int i = 0; i < NUM_THREADS; i++) {
			try{
				t[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		for (int i = 0; i < N; i++) {
			if(v[i]!= i*2) {
				System.out.println("Wrong answer" + v[i] + " " + i*2);
				System.exit(1);
			}
		}
		System.out.println("Correct");
	}

}
