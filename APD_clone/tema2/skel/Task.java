import java.nio.file.Path;
import java.util.ArrayList;

public class Task {
    // datele necesare fiecarui task
    //offsets, offset, path
    private String fileName;
    private int offset;
    private int size;
    private Path path;

    public Task(String fileName, int offset, int size, Path path) {
        this.fileName = fileName;
        this.offset = offset;
        this.size = size;
        this.path = path;
    }

    public String getFileName() {
        return fileName;
    }

    public int getOffset() {
        return offset;
    }

    public int getSize() {
        return size;
    }

    public Path getPath() {
        return path;
    }
}
