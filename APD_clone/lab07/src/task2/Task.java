package task2;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicInteger;

public class Task implements Runnable{
    private int[] colors;
    private int step;
    ExecutorService tpe;
    AtomicInteger submitted;

    public Task(int[] colors, int step, ExecutorService tpe) {
        this.colors = colors;
        this.step = step;
        this.tpe = tpe;
        submitted = new AtomicInteger(1);
    }

    private Task(int[] colors, int step, ExecutorService tpe, AtomicInteger submitted) {
        this.colors = colors;
        this.step = step;
        this.tpe = tpe;
        this.submitted = submitted;
    }

    private static boolean verifyColors(int[] colors, int step) {
        for (int i = 0; i < step; i++) {
            if (colors[i] == colors[step] && isEdge(i, step))
                return false;
        }
        return true;
    }

    private static boolean isEdge(int a, int b) {
        for (int[] ints : Main.graph) {
            if (ints[0] == a && ints[1] == b)
                return true;
        }
        return false;
    }

    static void printColors(int[] colors) {
        StringBuilder aux = new StringBuilder();
        for (int color : colors) {
            aux.append(color).append(" ");
        }
        System.out.println(aux);
    }
    @Override
    public void run() {
        if (step == Main.N) {
            printColors(colors);
            if(submitted.decrementAndGet() == 0) {
                tpe.shutdown();
            }
            return;
        }

        // for the node at position step try all possible colors
        for (int i = 0; i < Main.COLORS; i++) {
            int[] newColors = colors.clone();
            newColors[step] = i;
            if (verifyColors(newColors, step)) {
                int[] newColors2 = newColors;
                newColors2[i] = i;
                tpe.submit(new task2.Task(newColors2, step + 1, tpe, submitted));
                //colorGraph(newColors, step + 1);
            }
        }
    }
}
