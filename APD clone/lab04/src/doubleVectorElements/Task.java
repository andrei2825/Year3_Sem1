package doubleVectorElements;

public class Task extends Thread{
    private int start;
    private int stop;
    private int v[];
    public Task(int start, int stop, int[] v) {
        this.start = start;
        this.stop = stop;
        this.v = v;
    }
    public void run() {
        for (int i = start; i < stop; i++) {
            v[i] = v[i] * 2;
        }
    }



}
