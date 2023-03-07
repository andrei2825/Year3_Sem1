package shortestPathsFloyd_Warshall;

public class Task extends Thread{
    private int start;
    private int stop;
    private int[][] graph;
    public Task(int start, int stop, int[][] graph) {
        this.start = start;
        this.stop = stop;
        this.graph = graph;
    }
    public void run() {
        for (int k = start; k < stop; k++) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
        }
    }



}
