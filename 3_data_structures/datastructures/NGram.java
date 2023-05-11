package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.IDeque;

import java.util.Iterator;

public class NGram implements Iterable<String>, Comparable<NGram> {
    public static final String NO_SPACE_BEFORE = ",?!.-,:'";
    public static final String NO_SPACE_AFTER = "-'><=";
    public static final String REGEX_TO_FILTER = "”|\"|“|\\(|\\)|\\*";
    public static final String DELIMITER = "\\s+|\\s*\\b\\s*";
    private IDeque<String> data;

    public static String normalize(String s) {
        return s.replaceAll(REGEX_TO_FILTER, "").strip();
    }

    public NGram(IDeque<String> x) {
        this.data = new LinkedDeque<>();
        for (int i = 0; i < x.size(); i++) {
            this.data.addBack(x.peekFront());
            x.addBack(x.removeFront());
        }
    }

    public NGram(String data) {
        this(normalize(data).split(DELIMITER));
    }

    public NGram(String[] data) {
        this.data = new LinkedDeque<>();
        for (String s : data) {
            s = normalize(s);
            if (!s.isEmpty()) {
                this.data.addBack(s);
            }
        }
    }

    public NGram next(String word) {
        String[] data = new String[this.data.size()];
        for (int i = 0; i < data.length - 1; i++) {
            this.data.addBack(this.data.removeFront());
            data[i] = this.data.peekFront();
        }
        this.data.addBack(this.data.removeFront());
        data[data.length - 1] = word;
        return new NGram(data);
    }

    @Override
    public String toString() {
        String result = "";
        String prev = "";
        for (String s : this.data) {
            result += ((NO_SPACE_AFTER.contains(prev) || NO_SPACE_BEFORE.contains(s) || result.isEmpty()) ?  "" : " ") + s;
            prev = s;
        }
        return result.strip();
    }

    @Override
    public Iterator<String> iterator() {
        return this.data.iterator();
    }

    public int size(){
        return this.data.size();
    }
    @Override
    public int compareTo(NGram other) {
        if(this.equals(other)){
            return 0;
        }
        if(this.size()<other.size()){
            return -1;
        }
        else if (other.size()<this.size()){
            return 1;
        }
        else{
            NGram tClone = new NGram(this.data);
            NGram oClone = new NGram(other.data);
            return compareTo(tClone, oClone);
        }
    }

    private int compareTo(NGram a, NGram b){
        if(a.data == null || b.data == null){
            return 0;
        }
        String tFirst = a.data.peek();
        String oFirst = b.data.peek();
        if(!tFirst.equals(oFirst)){
            return tFirst.compareTo(oFirst);
        }
        a.data.removeFront();
        b.data.removeFront();
        return compareTo(a, b);
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof NGram)) {
            return false;
        }
        NGram other = (NGram) o;
        if(this.size() != other.size()){
            return false;
        }
        NGram tClone = new NGram(this.data);
        NGram oClone = new NGram(other.data);
        return equals(tClone, oClone);
    }

    private boolean equals(NGram a, NGram b){
        if(a.data.isEmpty() && b.data.isEmpty()){
            return true;
        }
        String aFirst = a.data.peekFront();
        String bFirst = b.data.peekFront();
        if(!aFirst.equals(bFirst)){
            return false;
        }
        a.data.removeFront();
        b.data.removeFront();
        return equals(a,b);
    }

    @Override
    public int hashCode() {
        int result = 17;
        for(String word : this.data){
            int fHash = word.hashCode();
            result = 31 * result + fHash;
        }
        return result;
    }
}
