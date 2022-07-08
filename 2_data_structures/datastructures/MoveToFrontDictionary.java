package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.ICollection;
import edu.caltech.cs2.interfaces.IDictionary;
import edu.caltech.cs2.interfaces.ISet;

import java.util.Iterator;

public class MoveToFrontDictionary<K, V> implements IDictionary<K,V> {
    private static class Node<K, V>{
        public final K KEY;
        public final V VALUE;
        public Node<K, V> next;
        public Node(K key, V value, Node<K, V> next){
            this.KEY = key;
            this.VALUE = value;
            this.next = next;
        }
    }
    private Node<K, V> head;
    private int size;
    public MoveToFrontDictionary() {
        this.head = null;
        this.size = 0;
    }

    @Override
    public V remove(K key) {
        if(this.head == null){
            return null;
        }
        else{
            Node<K, V> curr = this.head;
            Node<K, V> prev = this.head;
            while(curr.next != null){
                if(curr.KEY.equals(key)){
                    break;
                }
                prev = curr;
                curr = curr.next;
            }
            if(!curr.KEY.equals(key)){
                return null;
            }
            if(curr.equals(this.head)){
                this.head = curr.next;
            }
            else {
                prev.next = curr.next;
            }
            size--;
            return curr.VALUE;
        }
    }

    @Override
    public V put(K key, V value) {
        V result = remove(key);
        if(this.head == null){
            this.head = new Node(key, value, null);
            size++;
            return result;
        }
        Node<K, V> next = new Node(this.head.KEY, this.head.VALUE, this.head.next);
        this.head = new Node<>(key, value, next);
        size++;
        return result;
    }

    @Override
    public boolean containsKey(K key) {
        return this.get(key) != null;
    }

    @Override
    public boolean containsValue(V value) {
        return this.values().contains(value);
    }

    @Override
    public int size() {
        return this.size;
    }

    @Override
    public ICollection<K> keys() {
        ICollection<K> keys = new LinkedDeque<>();
        Node<K,V> curr = head;
        if(head == null){
            return keys;
        }
        while(curr.next != null){
            keys.add(curr.KEY);
            curr = curr.next;
        }
        keys.add(curr.KEY);
        return keys;
    }

    @Override
    public ICollection<V> values() {
        ICollection<V> values = new LinkedDeque<>();
        Node<K,V> curr = head;
        if(head == null){
            return values;
        }
        while(curr.next != null){
            values.add(curr.VALUE);
            curr = curr.next;
        }
        values.add(curr.VALUE);
        return values;
    }

    public V get(K key) {
        if(size() == 0){
            return null;
        }
        Node<K,V> curr = this.head;
        Node<K,V> prev = this.head;
        for(int i = 0; i<size;i++){
            if(curr.KEY.equals(key)){
                if(!curr.equals(this.head)){
                    prev.next = curr.next;
                    curr.next = this.head;
                    this.head = curr;
                }
                return curr.VALUE;
            }
            prev = curr;
            curr = curr.next;
        }
        return null;
    }

    public String toString(){
        Node<K, V> curr = this.head;
        String result = "[";
        if(curr == null){
            result = "[]";
        }
        else{
            while(curr.next!=null){
                result += "("+curr.KEY+", "+curr.VALUE+"), ";
                curr = curr.next;
            }
            result += "("+curr.KEY+", "+curr.VALUE+"), ";
            if(result.length()>=2) {
                result = result.substring(0, result.length() - 2);
            }
            result += "]";
        }
        return result;
    }

    @Override
    public ISet<K> keySet() {
        return IDictionary.super.keySet();
    }

    @Override
    public Iterator<K> iterator() {
        return new MoveToFrontDictionaryIterator();
    }
    private class MoveToFrontDictionaryIterator implements Iterator<K>{
        private int currIdx;

        MoveToFrontDictionaryIterator(){
            this.currIdx = 0;
        }
        @Override
        public boolean hasNext() {
            return this.currIdx<(MoveToFrontDictionary.this).size();
        }

        @Override
        public K next() {
            ICollection<K> keys = (MoveToFrontDictionary.this).keys();
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

    public static void main(String[] args){
        MoveToFrontDictionary<String, Integer> test = new MoveToFrontDictionary<>();
        System.out.println(test.put("a", 1));
        System.out.println(test);
        System.out.println(test.put("a", 100));
        System.out.println(test);
        test.put("a b", 2);
        System.out.println(test);
        test.put("a b c d e", 5);
        System.out.println(test);
        test.put("a b c", 3);
        System.out.println(test);
        test.put("a b c d", 4);
        System.out.println(test);
        System.out.println(test.keys());
        System.out.println(test.values());
    }
}
