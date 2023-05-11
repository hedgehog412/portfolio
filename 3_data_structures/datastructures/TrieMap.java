package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.ICollection;
import edu.caltech.cs2.interfaces.IDeque;
import edu.caltech.cs2.interfaces.ITrieMap;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.function.Function;

public class TrieMap<A, K extends Iterable<A>, V> implements ITrieMap<A, K, V> {
    private TrieNode<A, V> root;
    private Function<IDeque<A>, K> collector;
    private int size;

    public TrieMap(Function<IDeque<A>, K> collector) {
        this.root = null;
        this.collector = collector;
        this.size = 0;
    }

    @Override
    public boolean isPrefix(K key) {
        if (root == null) {
            return false;
        }
        TrieNode<A, V> curr = root;
        for (A letter : key) {
            if(curr == null){
                return false;
            }
            else if (!curr.pointers.containsKey(letter)) {
                return false;
            }
            else{
                curr = curr.pointers.get(letter);
            }
        }
        return true;
    }



    @Override
    public void clear() {
        root = null;
        size = 0;
    }

    @Override
    public V get(K key) {
        return get(key, root, 0);
    }

    private V get(K key, TrieNode<A, V> curr, int num) {
        A first = null;
        int n = 0;
        if (curr == null) {
            return null;
        }
        if (key == null) {
            return curr.value;
        } else {
            for (A letter : key) {
                if (n < num) {
                    n++;
                } else {
                    first = letter;
                    break;
                }
            }
            if (first == null) {
                return curr.value;
            }
            num = num + 1;
            return get(key, curr.pointers.get(first), num);
        }
    }

    @Override
    public V remove(K key) {
        if(root == null){
            return null;
        }
        else if (!containsKey(key)){
            return null;
        }
        else{
            if(key == null) {
                return null;
            }
            V result;
            TrieNode<A, V> curr = root;
            TrieNode<A, V> prev = new TrieNode<>();
            A let = null;
            for(A letter : key){
                let = letter;
                if(!curr.pointers.containsKey(letter)){
                    return null;
                }
                else{
                    curr = curr.pointers.get(letter);
                }
            }

            TrieNode<A, V> temp = root;
            for(A letter : key){
                if(letter.equals(let)){
                    prev.value = temp.value;
                    for(A k : temp.pointers.keySet()){
                        prev.pointers.put(k, temp.pointers.get(k));
                    }
                }
                temp = temp.pointers.get(letter);
            }
            prev.pointers.remove(let);
            result = curr.value;
            curr.value = null;
            remove(curr);
            size --;
            return result;
        }
    }

    private void remove(TrieNode<A, V> node){
        if(node == null){

        }
        else {
            //node.value = null;
            IDeque<A> copy = new LinkedDeque<>();
            for(A letter : node.pointers.keySet()){
                copy.add(letter);
            }
            for (A letter : copy) {
                remove(node.pointers.get(letter));
                node.pointers.remove(letter);
            }
        }
    }

    public String toString(){
        return root.toString();
    }
    @Override
    public V put(K key, V value) {
        size++;
        if (root == null) {
            root = new TrieNode<A, V>();
        }
        return put(key, value, root, 0);
    }

    private V put(K key, V value, TrieNode<A, V> node, int num) {
        A first = null;
        int n = 0;
        for (A letter : key) {
            if (n < num) {
                n++;
            } else {
                first = letter;
                break;
            }
        }
        if (first == null) {
            V old = node.value;
            if(old != null){
                size--;
            }
            node.value = value;
            return old;
        }
        if (!node.pointers.containsKey(first)) {
            node.pointers.put(first, new TrieNode<A, V>());
        }
        num = num + 1;
        return put(key, value, node.pointers.get(first), num);
    }

    @Override
    public boolean containsKey(K key) {
        TrieNode<A, V> curr = root;
        for(A letter : key){
            if(curr == null){
                return false;
            }
            if(!curr.pointers.containsKey(letter)){
                return false;
            }
            curr = curr.pointers.get(letter);
        }
        return curr.value != null;
    }

    @Override
    public boolean containsValue(V value) {
        TrieNode<A, V> curr = root;
        if(root == null){
            return false;
        }
        return containsValue(value, curr);
    }

    public boolean containsValue(V value, TrieNode<A, V> curr){
        if(curr.value == value){
            return true;
        }
        else {
            for (A letter : curr.pointers.keySet()) {
                return containsValue(value, curr.pointers.get(letter));
            }
            return true;
        }
    }

    @Override
    public int size() {
        return this.size;
    }

    @Override
    public IDeque<V> getCompletions(K prefix) {
        IDeque<V> result = new LinkedDeque<>();
        TrieNode<A, V> curr = root;
        if(!isPrefix(prefix)){
            return result;
        }
        for (A letter : prefix) {
            if (curr.pointers.get(letter) != null) {
                curr = curr.pointers.get(letter);
            }
        }
        getCompletions(result, curr);
        return result;
    }

    private void getCompletions(ICollection<V> result, TrieNode<A, V> node) {
        if (node != null) {
            if (node.value != null) {
                result.add(node.value);
            }
            for (A letter : node.pointers.keySet()) {
                getCompletions(result, node.pointers.get(letter));
            }
        }
    }

    @Override
    public ICollection<K> keys() {
        IDeque<K> result = new LinkedDeque<>();
        TrieNode<A, V> curr = root;
        IDeque<A> word = new LinkedDeque<>();
        keys(curr, result, word);
        return result;
    }

    private void keys(TrieNode<A,V> curr, IDeque<K> result, IDeque<A> word){
        if(curr != null){
            if (curr.value != null && word != null) {
                //System.out.println("word:"+collector.apply(word));
                IDeque<A> copy = new LinkedDeque<>();
                for(A letter : word){
                    copy.add(letter);
                }
                K finWord = collector.apply(copy);
                result.add(finWord);
                //System.out.println("Collection:"+result);
            }
            for (A letter : curr.pointers.keySet()) {
                word.add(letter);
                keys(curr.pointers.get(letter), result, word);
                word.removeBack();
            }
        }
    }

    @Override
    public ICollection<V> values() {
        ICollection<V> result = new LinkedDeque<>();
        TrieNode<A, V> curr = root;
        values(result, curr);
        return result;
    }

    private void values(ICollection<V> result, TrieNode<A, V> curr){
        if(curr != null){
            if(curr.value != null){
                result.add(curr.value);
            }
            for(A letter : curr.pointers.keySet()){
                values(result, curr.pointers.get(letter));
            }
        }
    }

    @Override
    public Iterator<K> iterator() {
        return new Iterator<K>() {
            private int idx = 0;
            private ICollection<K> keys = keys();
            @Override
            public boolean hasNext() {
                return idx < size();
            }

            @Override
            public K next() {
                int n = 0;
                idx++;
                for (K key : keys) {
                    if (n == idx -1) {
                        return key;
                    }
                    n++;
                }
                return null;
            }
        };
    }

    private static class TrieNode<A, V> {
        public final Map<A, TrieNode<A, V>> pointers;
        public V value;

        public TrieNode() {
            this(null);
        }

        public TrieNode(V value) {
            this.pointers = new HashMap<>();
            this.value = value;
        }

        @Override
        public String toString() {
            StringBuilder b = new StringBuilder();
            if (this.value != null) {
                b.append("[" + this.value + "]-> {\n");
                this.toString(b, 1);
                b.append("}");
            } else {
                this.toString(b, 0);
            }
            return b.toString();
        }

        private String spaces(int i) {
            StringBuilder sp = new StringBuilder();
            for (int x = 0; x < i; x++) {
                sp.append(" ");
            }
            return sp.toString();
        }

        protected boolean toString(StringBuilder s, int indent) {
            boolean isSmall = this.pointers.entrySet().size() == 0;

            for (Map.Entry<A, TrieNode<A, V>> entry : this.pointers.entrySet()) {
                A idx = entry.getKey();
                TrieNode<A, V> node = entry.getValue();

                if (node == null) {
                    continue;
                }

                V value = node.value;
                s.append(spaces(indent) + idx + (value != null ? "[" + value + "]" : ""));
                s.append("-> {\n");
                boolean bc = node.toString(s, indent + 2);
                if (!bc) {
                    s.append(spaces(indent) + "},\n");
                } else if (s.charAt(s.length() - 5) == '-') {
                    s.delete(s.length() - 5, s.length());
                    s.append(",\n");
                }
            }
            if (!isSmall) {
                s.deleteCharAt(s.length() - 2);
            }
            return isSmall;
        }

    }
}
