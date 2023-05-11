package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.ICollection;
import edu.caltech.cs2.interfaces.IDeque;
import edu.caltech.cs2.interfaces.IDictionary;

import java.util.Iterator;
import java.util.function.Supplier;

public class ChainingHashDictionary<K, V> implements IDictionary<K, V> {
    private Supplier<IDictionary<K, V>> chain;
    private IDictionary<K, V>[] table;
    private int size;
    private int load;
    private IDeque<Integer> primes;
    private int idx;

    public ChainingHashDictionary(Supplier<IDictionary<K, V>> chain) {
        this.primes = new LinkedDeque<>();
        this.primes.add(23);
        this.primes.add(53);
        this.primes.add(101);
        this.primes.add(239);
        this.primes.add(601);
        this.primes.add(1289);
        this.primes.add(2551);
        this.primes.add(5657);
        this.primes.add(12653);
        this.primes.add(25229);
        this.primes.add(51473);
        this.primes.add(113809);
        this.primes.add(240007);
        this.primes.add(399989);
        this.primes.add(400031);
        this.idx = 0;
        this.table = new IDictionary[11];
        primes.removeFront();
        idx++;
        size = 0;
        load = 0;
        this.chain = chain;
    }


    /**
     * @param key
     * @return value corresponding to key
     */
    @Override
    public V get(K key) {
       // System.out.println("b");
        int hash = key.hashCode() % table.length;
        if(hash<0){
            hash += this.table.length;
        }
        IDictionary<K, V> bucket = this.table[hash];
        if(bucket == null){
            return null;
        }
        return bucket.get(key);
    }

    @Override
    public V remove(K key) {
        //System.out.println("c");
        int hash = key.hashCode() % table.length;
        if(hash<0){
            hash += this.table.length;
        }
        IDictionary bucket = this.table[hash];
        if(bucket == null){
            return null;
        }
        V result = (V) bucket.remove(key);
        if(result != null){
            size--;
        }
        return result;
    }

    private void resize(){
        //System.out.println("d");
        load = (size+1) / this.table.length;
        if(load > 1){
            IDictionary[] newTable = new IDictionary[this.primes.peek()];
            if(this.primes.size() == 1){
                this.primes.add(this.primes.peek()*2);
            }
            this.primes.removeFront();
            for(int i = 0; i<this.table.length; i++){
                IDictionary<K,V> bucket = this.table[i];
                if(bucket != null && !bucket.isEmpty()){
                    for(K key : bucket.keys()){
                        int hash = key.hashCode() % newTable.length;
                        if(hash<0){
                            hash += newTable.length;
                        }
                        if(newTable[hash]==null){
                            newTable[hash] = this.chain.get();
                        }
                        newTable[hash].put(key, bucket.get(key));
                    }
                }
            }
            this.table = newTable;
        }
    }

    @Override
    public V put(K key, V value) {
        //System.out.println("e");
        resize();
        int hash = key.hashCode() % table.length;
        if(hash<0){
            hash += this.table.length;
        }
        IDictionary bucket = this.table[hash];
        if(bucket == null){
            bucket = this.chain.get();
            this.table[hash] = bucket;
        }
        size++;
        V result = (V) bucket.put(key, value);
        if(result!=null){
            size--;
        }
        return result;
    }

    @Override
    public boolean containsKey(K key) {
        //System.out.println("f");
        int hash = key.hashCode() % table.length;
        if(hash<0){
            hash += this.table.length;
        }
        IDictionary<K, V> bucket = this.table[hash];
        if(bucket == null){
            return false;
        }
        return bucket.containsKey(key);
    }

    /**
     * @param value
     * @return true if the HashDictionary contains a key-value pair with
     * this value, and false otherwise
     */
    @Override
    public boolean containsValue(V value) {
        //System.out.println("g");
        for(int i = 0; i< table.length;i++){
            if(table[i] != null){
                if(table[i].containsValue(value)){
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * @return number of key-value pairs in the HashDictionary
     */
    @Override
    public int size() {
        return this.size;
    }

    @Override
    public ICollection<K> keys() {
        //System.out.println("h");
        ICollection<K> keys = new LinkedDeque<>();
        for(int i = 0; i< table.length;i++){
            if(table[i] != null){
                for(Object key : table[i].keys()){
                    keys.add((K) key);
                }
            }
        }
        return keys;
    }

    @Override
    public ICollection<V> values() {
       // System.out.println("i");
        ICollection<V> values = new LinkedDeque<>();
        for(int i = 0; i< table.length;i++){
            if(table[i] != null){
                for(Object value : table[i].values()){
                    values.add((V) value);
                }
            }
        }
        return values;
    }

    /**
     * @return An iterator for all entries in the HashDictionary
     */
    @Override
    public Iterator<K> iterator() {
       // System.out.println("j");
        return this.keys().iterator();
    }

    private class ChainingHashDictionaryIterator implements Iterator<K>{
        private int currIdx;

        public ChainingHashDictionaryIterator(){
            this.currIdx = 0;
        }

        @Override
        public boolean hasNext() {
            return currIdx < (ChainingHashDictionary.this).size;
        }

        @Override
        public K next() {
            ICollection<K> keys = (ChainingHashDictionary.this).keys();
            int i = 0;
            for(K key : keys){
                if(i == currIdx){
                    currIdx++;
                    return key;
                }
                i++;
            }
            return null;
        }
    }
}
