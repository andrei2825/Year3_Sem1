
public class Reduce implements Runnable{
    private Res[] results;
    private int id;
    private int fileId;
    int[] lenPerFile;
    private int numberOfTasks;
    private Sol[] solutions;
    private int numberOfThreads;
    private int numberOfFiles;

    public Reduce(Res[] results, int id, int fileId, int[] lenPerFile, int numberOfTasks, Sol[] solutions, int numberOfThreads, int numberOfFiles) {
        this.results = results;
        this.id = id;
        this.fileId = fileId;
        this.lenPerFile = lenPerFile;
        this.numberOfTasks = numberOfTasks;
        this.solutions = solutions;
        this.numberOfThreads = numberOfThreads;
        this.numberOfFiles = numberOfFiles;
    }

    private int fibonacci(int n) {
        int n1 = 0,n2 = 1,n3 = 0, i;
        for(i = 2; i <= n+1; i++)
        {
            n3 = n1 + n2;
            n1 = n2;
            n2 = n3;
        }
        return n3;
    }

    @Override
    public void run() {
        // Fiecare worker lucreaza taskurile predestinate lui
        for(int l = id; l < numberOfFiles; l += numberOfThreads) {
            int i;
            int sum = 0;
            int numCuv = 0;
            // Start si end delimiteaza protiunea din arrayul de rezultate de la partea de mapare predestinata acestui task
            int start = 0;
            for (i = 0; i < l; i++) {
                start += lenPerFile[i];
            }
            int end = start + lenPerFile[l];
            int maxLen = 0;
            int numMaxLen = 0;
            //calculez suma, nr de cuvinte si lungimea maxima
            for (i = start; i < end; i++) {
                for (int j = 0; j < results[i].getWordCounter().size(); j++) {
                    sum += fibonacci(results[i].getWordCounter().get(j).getA()) *  results[i].getWordCounter().get(j).getB();
                    numCuv += results[i].getWordCounter().get(j).getB();
                    if(results[i].getWordCounter().get(j).getA() > maxLen) {
                        maxLen = results[i].getWordCounter().get(j).getA();
                    }
                }
            }
            //calculez numarul de cuvinte de lungime maxima
            for (i = start; i < end; i++) {
                for (int j = 0; j < results[i].getWordCounter().size(); j++) {
                    if(results[i].getWordCounter().get(j).getA() == maxLen) {
                        numMaxLen += 1 * results[i].getWordCounter().get(j).getB();
                    }
                }
            }
            //adaug rezultatele la solutie
            solutions[l].setRang((double) sum/numCuv);
            solutions[l].setMaxLen(maxLen);
            solutions[l].setNumberOfWords(numMaxLen);
        }
    }
}
