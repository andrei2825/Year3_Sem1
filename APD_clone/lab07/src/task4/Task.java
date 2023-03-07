package task4;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.RecursiveTask;

public class Task extends RecursiveTask<Void> {
    ArrayList<Integer> partialPath;
    int destination;
    public Task(ArrayList<Integer> partialPath, int destination){
        this.partialPath = partialPath;
        this.destination = destination;
    }
    @Override
    protected Void compute() {
        if (partialPath.get(partialPath.size() - 1) == destination) {
            System.out.println(partialPath);
            return null;
        }

            // se verifica nodurile pentru a evita ciclarea in graf
        int lastNodeInPath = partialPath.get(partialPath.size() - 1);
        List<Task> tasks = new ArrayList<>();
        for (int[] ints : Main.graph) {
            if (ints[0] == lastNodeInPath) {
                if (partialPath.contains(ints[1]))
                    continue;
                ArrayList<Integer> newPartialPath = new ArrayList<>(partialPath);
                newPartialPath.add(ints[1]);
                Task newTask = new Task(newPartialPath, destination);
                newTask.fork();
                tasks.add(newTask);

//                getPath(newPartialPath, destination);
            }
        }
        tasks.forEach(Task::join);
        return null;
    }
}
