public class Sol {
    // datele rezultate in urma operatiei de reducere 
    private String fileName;
    private double rang;
    private int maxLen;
    private int numberOfWords;

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public void setRang(double rang) {
        this.rang = rang;
    }

    public void setMaxLen(int maxLen) {
        this.maxLen = maxLen;
    }

    public void setNumberOfWords(int numberOfWords) {
        this.numberOfWords = numberOfWords;
    }

    public String getFileName() {
        return fileName;
    }

    public double getRang() {
        return rang;
    }

    public int getMaxLen() {
        return maxLen;
    }

    public int getNumberOfWords() {
        return numberOfWords;
    }
}
