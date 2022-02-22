package t1;

public class main {
    public static void main(String[] args) {
        int NUMBER_OF_THREADS = 4;
        Thread[] t = new Thread[NUMBER_OF_THREADS];

        for (int i = 0; i < NUMBER_OF_THREADS; i++) {
            t[i] = new Task(i);
            t[i].start();
        }
    }
}
