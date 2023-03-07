import java.io.*;
import java.nio.file.Path;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.*;


public class Mapper implements Runnable {
    private int offset;
    private int size;
    private Path path;
    private Res[] result;
    private int numThreads;
    private int id;
    private Task[] tasks;

    public Mapper(int id, Task[] tasks,
                  Res[] result, int numThreads) {
        this.id = id;
        this.tasks = tasks;
        this.result = result;
        this.numThreads = numThreads;
    }

    @Override
    public void run() {
        int i;
        int numTasks = tasks.length;
        ArrayList<Task> currentTasks = new ArrayList<>();
        ArrayList<Integer>resultId = new ArrayList<>();
        // Fiecare worker isi ia tasskurile destinate lui
        for ( i = id; i < numTasks; i+= numThreads) {
            currentTasks.add(tasks[i]);
            resultId.add(i);
        }
        // Prelucrare taskuri
        for(int l = 0; l < currentTasks.size(); l++) {
            String content = new String();
            path = currentTasks.get(l).getPath();
            offset = currentTasks.get(l).getOffset();
            size = currentTasks.get(l).getSize();
            ArrayList<String> wordList = new ArrayList<>();
            // Citesc continutul fisierului
            try {
                content = Files.readString(path, StandardCharsets.US_ASCII);
            } catch (IOException e) {
                e.printStackTrace();
            }
            int first = 0;
            if(offset > 0){
                // Verfic daca caracterul precedent este o litera sau o cifrea, caz in care 
                // iterez prin caractere pana la primul caracter non litera /cifra, unde mut offsetul si ajustez size-ul
                if (Character.isDigit(content.charAt(offset - 1)) || Character.isLetter(content.charAt(offset - 1))) {
                    if(offset + size >= content.length()) {
                        size = content.length() - offset;
                    }
                    for (i = offset - 1; i < offset + size - 1; i++) {
                        if (!Character.isDigit(content.charAt(i)) && !Character.isLetter(content.charAt(i))){
                            break;
                        }
                        offset += 1;
                        size--;
                        if(size == 0) {
                            break;
                        }
                    }
                }
            }
            if(offset + size >= content.length()) {
                size = content.length() - offset;
            }
            // Asemanator doar ca pentru finalul stringului
            else {
                if (Character.isDigit(content.charAt(offset + size - 1)) || Character.isLetter(content.charAt(offset + size - 1))) {
                    for (i = offset + size; i < content.length(); i++) {
                        if (!Character.isDigit(content.charAt(i)) && !Character.isLetter(content.charAt(i))){
                            break;
                        }
                        size++;
                    }
                }
            }
            String workingContent = content.substring(offset, offset + size);
            // Separ continutul in cuvinte
            String[] splitWords = workingContent.split("\\W+");
            for(i = 0; i < splitWords.length; i++) {
                wordList.add(splitWords[i]);
            }
            // iau fiecare cuvant si calculez datele necesare
            while(!wordList.isEmpty()) {
                int counter = 0;
                int elementLength = wordList.get(0).length();
                for(i =0; i < wordList.size(); i++) {
                    if(wordList.get(i).length() > elementLength) {
                        elementLength = wordList.get(i).length();
                    }
                }
                for(i =0; i < wordList.size(); i++) {
                    if(wordList.get(i).length() == elementLength) {
                        if(first == 0) {
                            result[resultId.get(l)].getLongestWord().add(wordList.get(i));
                        }
                        counter += 1;
                        wordList.remove(i);
                        i--;
                    }
                }
                first = 1;
                if(elementLength > 0) {
                    result[resultId.get(l)].getWordCounter().add(new Pair(elementLength, counter));
                }
            }
        }
    }
}