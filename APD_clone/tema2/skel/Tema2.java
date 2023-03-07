import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.text.DecimalFormat;
import java.util.*;


public class Tema2 {
    public static void main(String[] args) {
        int numThreads = 0;
        int offset = 0;
        int docNumber = 0;
        int len = 0;
        int[] lenPerFile;
        File f;
        ArrayList<String>files = new ArrayList<>();
        String inFile;
        String outFile;
        DecimalFormat rank = new DecimalFormat("0.00");
        if (args.length < 3) {
            System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            return;
        }
        // preluare argumente
        numThreads = Integer.valueOf(args[0]);
        inFile = args[1];
        outFile = args[2];
        //Citesc din fisierul de intrare offsetul, numarul de fisiere input si path-urile catre aceste fisiere
        try {
            File inputFile = new File(inFile);
            Scanner myReader = new Scanner(inputFile);
            offset = Integer.parseInt(myReader.nextLine());
            docNumber = Integer.parseInt(myReader.nextLine());
            while(myReader.hasNextLine()) {
                files.add(myReader.nextLine());
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        lenPerFile = new int[docNumber];
        ArrayList<Integer> offsets = new ArrayList<>();
        // Calculez lungimea in caractere a continutului fiecarui fisier
        for (int i = 0; i < docNumber; i++) {
            try {
                String content = Files.readString( Paths.get(files.get(i)), StandardCharsets.US_ASCII);
                len += content.length() / offset;
                lenPerFile[i] = content.length() / offset;
                int tmp = content.length();
                int tmpOffset = 0;
                while (tmp > 0) {
                    offsets.add(tmpOffset);
                    tmpOffset += offset;
                    tmp -= offset;
                }
                if(content.length() % offset != 0) {
                    len += 1;
                    lenPerFile[i] += 1;
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        Res[] result = new Res[len];
        Task[] tasks = new Task[len];
        int count = 0;
        // Pupulez lista de task-uri
        for (int i = 0; i < docNumber; i++) {
            for (int j = 0; j < lenPerFile[i]; j++) {
                f = new File(Paths.get(files.get(i)).toString());
                result[count] = new Res(f.getName());
                tasks[count] = new Task(f.getName(), offsets.get(count), offset, Paths.get(files.get(i)));
                count += 1;
            }
        }
        // Creez lista de solutii finale
        Sol[] solutions = new Sol[docNumber];
        for (int i = 0; i < docNumber; i++) {
            solutions[i] = new Sol();
            f = new File(Paths.get(files.get(i)).toString());
            solutions[i].setFileName(f.getName());
        }

        Thread []threads = new Thread[numThreads];

        // Pornire workeri pentru a face maparea
        for (int i = 0; i < numThreads; i++) {
            threads[i] = new Thread(new Mapper(i, tasks, result, numThreads));
            threads[i].start();
        }
        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        Thread []reducers = new Thread[numThreads];
        int fileId = 0;
        int start = 0;
        // Pornire workeri pentru a face reducerea
        for (int i = 0; i < numThreads; i++) {
            reducers[i] = new Thread(new Reduce(result, i, fileId, lenPerFile, lenPerFile[fileId], solutions, numThreads, docNumber));
            reducers[i].run();
        }
        for (int i = 0; i < numThreads; i++) {
            try {
                reducers[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        //Calculare ordine rezultate
        ArrayList<Sol>finalSolutions = new ArrayList<>();
        for (int i = 0; i < solutions.length; i++) {
            finalSolutions.add(solutions[i]);
        }
        double maxRank = 0;
        for (int i = 0; i < finalSolutions.size(); i++) {
            if (maxRank < finalSolutions.get(i).getRang()) {
                maxRank = finalSolutions.get(i).getRang();
            }
        }
        //Scriere in fisierul de iesire
        try {
            FileWriter writer = new FileWriter(outFile);
            while (finalSolutions.size() > 0) {
                for (int i = 0; i < finalSolutions.size(); i++) {
                    if (maxRank == finalSolutions.get(i).getRang()) {
                        writer.write(finalSolutions.get(i).getFileName() + ","
                                + String.format("%.2f", finalSolutions.get(i).getRang()) + ","
                                + finalSolutions.get(i).getMaxLen() + ","
                                + finalSolutions.get(i).getNumberOfWords());
                        finalSolutions.remove(i);
                        maxRank = 0;
                        if(finalSolutions.size() > 0) {
                            writer.write("\n");
                        }
                    }
                }
                for (int i = 0; i < finalSolutions.size(); i++) {
                    if (maxRank < finalSolutions.get(i).getRang()) {
                        maxRank = finalSolutions.get(i).getRang();
                    }
                }
            }
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
