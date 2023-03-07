import java.util.ArrayList;

public class Res {
    // datele rezultate in urma operatiei de mapare
    private String fileName;
    private ArrayList<Pair> wordCounter = new ArrayList<>();
    private ArrayList<String> longestWord = new ArrayList<>();

    public Res(String fileName) {
        this.fileName = fileName;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public ArrayList<Pair> getWordCounter() {
        return wordCounter;
    }

    public void setWordCounter(ArrayList<Pair> wordCounter) {
        this.wordCounter = wordCounter;
    }

    public ArrayList<String> getLongestWord() {
        return longestWord;
    }

    public void setLongestWord(ArrayList<String> longestWord) {
        this.longestWord = longestWord;
    }
}
