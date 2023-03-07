package task5;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class Task extends RecursiveTask<Void> {
    private int[] colors;
    private int step;
    public Task (int[] colors, int step) {
        this.colors = colors;
        this.step = step;
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
    protected Void compute() {
        if (step == Main.N) {
            printColors(colors);
            return null;
        }

        List<Task> tasks = new ArrayList<>();

        // for the node at position step try all possible colors
        for (int i = 0; i < Main.COLORS; i++) {
            int[] newColors = colors.clone();
            newColors[step] = i;
            if (verifyColors(newColors, step)) {
                Task t = new Task(newColors, step + 1);
                t.fork();
                tasks.add(t);
            }
        }
        tasks.forEach(Task::join);
        return null;
    }
}
