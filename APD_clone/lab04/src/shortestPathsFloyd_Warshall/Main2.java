package shortestPathsFloyd_Warshall;

/**
 * @author cristian.chilipirea
 *
 */
public class Main2 {

    public static void main(String[] args) {
        int NUM_THREADS = 4;
        int start;
        int stop;
        int M = 9;
        int graph[][] = { { 0, 1, M, M, M },
                          { 1, 0, 1, M, M },
                          { M, 1, 0, 1, 1 },
                          { M, M, 1, 0, M },
                          { M, M, 1, M, 0 } };
        Thread[] t = new Thread[NUM_THREADS];

        // Parallelize me (You might want to keep the original code in order to compare)

        for (int i = 0; i < NUM_THREADS; i++) {
            start = (int)(i * (double)5/NUM_THREADS);
            stop = (int)Math.min((i+1)*(double)5/NUM_THREADS, 5);
            t[i] = new Task(start, stop, graph);
            t[i].start();
        }
        for (int i = 0; i < NUM_THREADS; i++) {
            try{
                t[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }
    }
}
