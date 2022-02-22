import java.io.*;
import java.util.*;

class Graph {
    private LinkedList<Integer> adjLists[];
    private ArrayList<Integer> symbols = new ArrayList<>();
    private ArrayList<ArrayList<Integer>> paths = new ArrayList<>();
    private boolean visited[];
    private int size;
    public int index;


    Graph(int n) {
        index = 0;
        adjLists = new LinkedList[n];
        visited = new boolean[n];
        this.size = n;

        for (int i = 0; i < n; i++) {
            adjLists[i] = new LinkedList<Integer>();
        }
    }
    public void printEdges(int n, int m) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                System.out.print(adjLists[i].get(j) + " ");
            }
            System.out.println(" ");
        }
    }
    void addEdge(int src, int dest) {
        adjLists[src].add(dest);
    }

    public void getAllPaths(int s, int d) {
        boolean[] isVisited = new boolean[size];
        ArrayList<Integer> currentPath = new ArrayList<>();
        currentPath.add(s);
        getAllPathsHelper(s, d, isVisited, currentPath);
        System.out.println(s + " " + d);
        System.out.println(currentPath);
        paths.add(currentPath);
    }

    private void getAllPathsHelper(Integer u, Integer d, boolean[] isVisited, List<Integer> localPathList) {
        if (u.equals(d)) {
            return;
        }
        isVisited[u] = true;
        int k = 0;
        for (Integer i : adjLists[u]) {
            if (!isVisited[i]) {
                localPathList.add(k++);
                getAllPathsHelper(i, d, isVisited, localPathList);
                localPathList.add(-1);
            }
        }
        isVisited[u] = false;
    }


    void DFS(int v) {
        visited[v] = true;
        Iterator<Integer> ite = adjLists[v].listIterator();
        while (ite.hasNext()) {
            int adj = ite.next();
            if (!visited[adj])
                DFS(adj);
        }
    }

    public void setVisited(boolean[] visited) {
        this.visited = visited;
    }
    public boolean[] getVisited() {
        return visited;
    }

    public LinkedList<Integer>[] getAdjLists() {
        return adjLists;
    }

    public ArrayList<ArrayList<Integer>> getPaths() {
        return paths;
    }
}


class Main{
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int n = scanner.nextInt();
        int m = scanner.nextInt();
        int s = scanner.nextInt();
        int f = scanner.nextInt();
        int [][]stari = new int[n][m];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                stari[i][j] = scanner.nextInt();
            }
        }

        int []initiale = new int[s];
        int []finale = new int[f];
        for (int i = 0 ; i < s; i++) {
            initiale[i] = scanner.nextInt();
        }
        for (int i = 0 ; i < f; i++) {
            finale[i] = scanner.nextInt();
        }

        scanner.close();
        Graph graph = new Graph(m*n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                graph.addEdge(i, stari[i][j]);
            }
        }
        boolean []resultA = new boolean[n];
        boolean []resultB = new boolean[n];
        switch (args[0]) {
            case "accessible":
                graph.setVisited(new boolean[m*n]);
                for (int i = 0; i < s; i++) {
                    graph.DFS(i);
                }
                boolean []visited = graph.getVisited();
                for (int i = 0; i < n; i++) {
                    if(visited[i] == true) {
                        resultA[i] = true;
                        System.out.println(i);
                    }
                }
                break;
            case "productive":
                graph.setVisited(new boolean[m*n]);
                for (int i = 0; i < n; i++) {
                    graph.DFS(i);
                    for (int j = 0; j < f; j++) {
                        if(graph.getVisited()[finale[j]] == true) {
                            resultB[i] = true;
                            System.out.println(i);
                            break;
                        }
                    }
                    graph.setVisited(new boolean[m*n]);
                }
                break;
            case "useful":
                graph.setVisited(new boolean[m*n]);
                for (int i = 0; i < s; i++) {
                    graph.DFS(i);
                }
                visited = graph.getVisited();
                for (int i = 0; i < n; i++) {
                    if(visited[i] == true) {
                        resultA[i] = true;
                    }
                }

                graph.setVisited(new boolean[m*n]);
                for (int i = 0; i < n; i++) {
                    graph.DFS(i);
                    for (int j = 0; j < f; j++) {
                        if(graph.getVisited()[finale[j]] == true) {
                            resultB[i] = true;
                            break;
                        }
                    }
                    graph.setVisited(new boolean[m*n]);
                }

                for (int i = 0; i < n; i++) {
                    if(resultA[i] && resultB[i]) {
                        System.out.println(i);
                    }
                }
                break;
            case "synchronize":
                    if(f == 0 && s != 0) {
                        LinkedList<Integer> adjLists[] = graph.getAdjLists();
                        int []state = new int[s];
                        boolean[][]vis = new boolean[n][m];
                        for (int i = 0; i < s; i++) {
                            state[i] = initiale[i];
                        }
                        boolean sync = false;
                        int sym = 0;
                        int []tmp2 = new int[s];
                        while(!sync && sym < m) {
                            boolean tmpOk = true;
                            for (int i = 0; i < s; i++) {
                                tmp2[i] = state[i];
                            }
                            for (int i = 0; i < s; i++) {
                                if (!vis[i][sym]) {
                                    state[i] = adjLists[i].get(sym);
                                } else {
                                    for (int j = 0; j < s; j++) {
                                        state[j] = tmp2[j];
                                    }
                                    sym++;
                                    break;
                                }
                                vis[i][sym] = true;
                            }
                            for (int i = 0; i < s; i++) {
                                if(state[i] != state[0]) {
                                    tmpOk = false;
                                }
                            }
                            if(tmpOk) {
                                sync = true;
                            }
                            if(sym < m) {
                                System.out.print(sym + " ");
                            }
                        }


                    } else if(s == 0 && f == 0) {
                        LinkedList<Integer> adjLists[] = graph.getAdjLists();
                        int []state = new int[n];
                        boolean[][]vis = new boolean[n][m];
                        for (int i = 0; i < n; i++) {
                            state[i] = i;
                        }
                        boolean sync = false;
                        int sym = 0;
                        int []tmp2 = new int[n];
                        while(!sync && sym < m) {
                            boolean tmpOk = true;
                            for (int i = 0; i < n; i++) {
                                tmp2[i] = state[i];
                            }
                            for (int i = 0; i < n; i++) {
                                if (!vis[i][sym]) {
                                    state[i] = adjLists[i].get(sym);
                                } else {
                                    for (int j = 0; j < n; j++) {
                                        state[j] = tmp2[j];
                                    }
                                    if(sym < m)
                                        sym++;
                                    else
                                        sym = 0;
                                    break;
                                }
                                vis[i][sym] = true;
                            }
                            for (int i = 0; i < n; i++) {
                                if(state[i] != state[0]) {
                                    tmpOk = false;
                                }
                            }
                            if(tmpOk) {
                                sync = true;
                            }
                            if(sym < m) {
                                System.out.print(sym + " ");
                            }

                        }
                    } else if(s != 0 && f != 0) {
                        LinkedList<Integer> adjLists[] = graph.getAdjLists();
                        int []state = new int[s];
                        boolean[][]vis = new boolean[n][m];
                        for (int i = 0; i < s; i++) {
                            state[i] = initiale[i];
                        }
                        boolean sync = false;
                        int sym = 0;
                        int []tmp2 = new int[s];
                        int total = 0;
                        while(!sync && sym < m) {
                            boolean tmpOk = true;
                            for (int i = 0; i < s; i++) {
                                tmp2[i] = state[i];
                            }
                            int tmpSym = sym;
                            while (sym < m) {
                                for (int i = 0; i < s; i++) {
                                    if (!vis[i][sym]) {
                                        state[i] = adjLists[i].get(sym);
                                    } else {
                                        for (int j = 0; j < s; j++) {
                                            state[j] = tmp2[j];
                                        }
                                        sym++;
                                        break;
                                    }
                                    vis[i][sym] = true;
                                }
                                for (int i = 0; i < s; i++) {
                                    if(state[i] != state[0]) {
                                        tmpOk = false;
                                    }
                                }
                                for (int i = 0; i <  s; i++) {
                                    for (int j = 0; j < f; j++) {
                                        if(state[i] == finale[j]) {
                                            total++;
                                            break;
                                        }
                                    }
                                }
                                if(tmpOk && total == s) {
                                    sync = true;
                                    break;
                                }
                            }
                            if(!sync) {
                                sym = tmpSym+1;
                            }
                            if(sym < m) {
                                System.out.print(sym + " ");
                            }
                        }
                    } else if(s == 0 && f != 0) {
                        LinkedList<Integer> adjLists[] = graph.getAdjLists();
                        int []state = new int[n];
                        boolean[][]vis = new boolean[n][m];
                        for (int i = 0; i < n; i++) {
                            state[i] = initiale[i];
                        }
                        boolean sync = false;
                        int sym = 0;
                        int []tmp2 = new int[n];
                        int total = 0;
                        while(!sync && sym < m) {
                            boolean tmpOk = true;
                            for (int i = 0; i < n; i++) {
                                tmp2[i] = state[i];
                            }
                            int tmpSym = sym;
                            while (sym < m) {
                                for (int i = 0; i < n; i++) {
                                    if (!vis[i][sym]) {
                                        state[i] = adjLists[i].get(sym);
                                    } else {
                                        for (int j = 0; j < n; j++) {
                                            state[j] = tmp2[j];
                                        }
                                        sym++;
                                        break;
                                    }
                                    vis[i][sym] = true;
                                }
                                for (int i = 0; i < n; i++) {
                                    if(state[i] != state[0]) {
                                        tmpOk = false;
                                    }
                                }
                                for (int i = 0; i <  n; i++) {
                                    for (int j = 0; j < f; j++) {
                                        if(state[i] == finale[j]) {
                                            total++;
                                            break;
                                        }
                                    }
                                }
                                if(tmpOk && total == n) {
                                    sync = true;
                                    break;
                                }
                            }
                            if(!sync) {
                                sym = tmpSym+1;
                            }
                            if(sym < m) {
                                System.out.print(sym + " ");
                            }
                        }
                    }
                break;
            default:
                break;
        }
    }
}



