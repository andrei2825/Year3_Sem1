package task3;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class Task implements Runnable {
    int[] graph;
    int step;
    ExecutorService tpe;
    AtomicInteger submitted;
    public Task(int[] graph, int step, ExecutorService tpe) {
        this.graph = graph;
        this.step =step;
        this.tpe = tpe;
        this.submitted = new AtomicInteger(1);
    }

    private Task(int[] graph, int step, ExecutorService tpe, AtomicInteger submitted) {
        this.graph = graph;
        this.step =step;
        this.tpe = tpe;
        submitted.incrementAndGet();
        this.submitted = submitted;
    }
    private static boolean check(int[] arr, int step) {
        for (int i = 0; i <= step; i++) {
            for (int j = i + 1; j <= step; j++) {
                if (arr[i] == arr[j] || arr[i] + i == arr[j] + j || arr[i] + j == arr[j] + i)
                    return false;
            }
        }
        return true;
    }

    private static void printQueens(int[] sol) {
        StringBuilder aux = new StringBuilder();
        for (int i = 0; i < sol.length; i++) {
            aux.append("(").append(sol[i] + 1).append(", ").append(i + 1).append("), ");
        }
        aux = new StringBuilder(aux.substring(0, aux.length() - 2));
        System.out.println("[" + aux + "]");
    }

    @Override
    public void run() {
        if (Main.N == step) {
            printQueens(graph);
            if(submitted.decrementAndGet() == 0) {
                tpe.shutdown();
            }
            return;
        }
        for (int i = 0; i < Main.N; ++i) {
            int[] newGraph = graph.clone();
            newGraph[step] = i;

            if (check(newGraph, step)) {
                tpe.submit(new Task(newGraph, step+1, tpe, submitted));
//                queens(newGraph, step + 1);
            }
        }
    }
}
