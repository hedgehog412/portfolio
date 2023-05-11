package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.IDictionary;
import edu.caltech.cs2.interfaces.IPriorityQueue;

import java.util.Iterator;

public class MinFourHeap<E> implements IPriorityQueue<E> {

    private static final int DEFAULT_CAPACITY = 10;

    private int size;
    private PQElement<E>[] data;
    private IDictionary<E, Integer> keyToIndexMap;

    /**
     * Creates a new empty heap with DEFAULT_CAPACITY.
     */
    public MinFourHeap() {
        this.size = 0;
        this.data = new PQElement[DEFAULT_CAPACITY];
        this.keyToIndexMap = new ChainingHashDictionary<>(MoveToFrontDictionary::new);
    }

    private void swap(int prev, int next){
        PQElement<E> temp = new PQElement<>(data[next].data, data[next].priority);
        data[next] = new PQElement<>(data[prev].data, data[prev].priority);
        data[prev] = temp;
        keyToIndexMap.put(temp.data, prev);
        keyToIndexMap.put(data[next].data, next);
    }


    private int findMinChild(int idx){
        if(idx*4+1>=size){
            return idx;
        }
        else{
            int minIdx = idx;
            if(data[minIdx].priority>data[idx*4+1].priority){
                minIdx = idx*4+1;
            }
            if(idx*4+2<size){
                if(data[minIdx].priority>data[idx*4+2].priority){
                    minIdx = idx*4+2;
                }
            }
            if(idx*4+3<size){
                if(data[minIdx].priority>data[idx*4+3].priority){
                    minIdx = idx*4+3;
                }
            }
            if(idx*4+4<size){
                if(data[minIdx].priority>data[idx*4+4].priority){
                    minIdx = idx*4+4;
                }
            }
            return minIdx;
        }
    }

    @Override
    public PQElement<E> dequeue() {
        if(size == 0){
            return null;
        }
        PQElement<E> result = new PQElement<>(data[0].data, data[0].priority);
        if(size == 1){
            keyToIndexMap.remove(data[0].data);
            data[0] = null;
            size--;
            return result;
        }
        data[0] = new PQElement<>(data[size-1].data, data[size-1].priority);
        data[size-1] = null;
        size--;
        keyToIndexMap.remove(result.data);
        keyToIndexMap.put(data[0].data,0);
        percolateDown(0);
        return result;
    }

    private void percolateDown(int idx){
        int minIdx = findMinChild(idx);
        while(idx != minIdx ){
            swap(idx, minIdx);
            idx = minIdx;
            minIdx = findMinChild(idx);
        }
    }

    @Override
    public boolean enqueue(PQElement<E> epqElement) {
        resize();
        if(keyToIndexMap.containsKey(epqElement.data)){
            throw new IllegalArgumentException();
        }
        size++;
        data[size - 1] = epqElement;
        keyToIndexMap.put(epqElement.data, size - 1);
        percolateUp(size - 1);
        return true;
    }

    private void percolateUp(int idx){
        int par;
        if(idx % 4 == 0){
            par = idx / 4 -1;
        }
        else{
            par = (idx - idx%4) /4;
        }
        while(idx != 0 &&data[par].priority > data[idx].priority){{
            swap(par, idx);
            idx = par;
            if(idx % 4 == 0){
                par = idx / 4 -1;
            }
            else{
                par = (idx - idx%4) /4;
            }
        }

        }
    }


    @Override
    public void increaseKey(PQElement<E> key){
        if(!this.keyToIndexMap.containsKey(key.data)){
            throw new IllegalArgumentException();
        }
        int idx = this.keyToIndexMap.get(key.data);
        data[idx] = key;
        percolateDown(idx);
    }

    @Override
    public void decreaseKey(PQElement<E> key){
        if(!this.keyToIndexMap.containsKey(key.data)){
            throw new IllegalArgumentException();
        }
        int idx = this.keyToIndexMap.get(key.data);
        data[idx] = key;
        percolateUp(idx);
    }

    private void resize(){
        if(size >= this.data.length){
            PQElement<E>[] temp = new PQElement[this.data.length*2];
            for(int i = 0 ; i<size;i++){
                temp[i] = new PQElement<>(this.data[i].data, this.data[i].priority);
            }
            this.data = temp;
        }
    }



    @Override
    public PQElement<E> peek() {
        if(this.size > 0){
            return data[0];
        }
        return null;
    }

    @Override
    public int size() {
        return this.size;
    }

    @Override
    public Iterator<PQElement<E>> iterator() {
        return null;
    }

    /*
    public String toString(){
        String result = "";
        for(int i = 0; i<size; i++){
            result += "(" + i + ", " + data[i] + ") ";
        }
        return result;
    }

    public static void main(String[] args) {
        MinFourHeap test = new MinFourHeap();
        test.enqueue(new PQElement(9, 9));
        System.out.println(test);
        test.enqueue(new PQElement(-100, -100));
        System.out.println(test);
        test.enqueue(new PQElement(7, 7));
        System.out.println(test);
        test.enqueue(new PQElement(19, 19));
        System.out.println(test);
        test.enqueue(new PQElement(3, 3));
        System.out.println(test);
        test.enqueue(new PQElement(-2, -2));
        System.out.println(test);
        test.enqueue(new PQElement(1, 1));
        System.out.println(test);
        test.enqueue(new PQElement(-84, -84));
        System.out.println(test);
        test.enqueue(new PQElement(-4, -4));
        System.out.println(test);
        test.enqueue(new PQElement(2, 2));
        System.out.println(test);
        test.enqueue(new PQElement(70, 70));
        System.out.println(test);

        while (test.size() > 0) {
            System.out.println(test.dequeue());
            System.out.println(test);
        }
        /*
        for (Object temp : test.keyToIndexMap.keys()) {
            System.out.println(temp + " , " + test.keyToIndexMap.get(temp));
        }
        System.out.println(test.size);

        test.decreaseKey(new PQElement(7, -105));
        System.out.println(test);

        int i = test.size;
        while(i>0) {
            i--;
            test.dequeue();
            System.out.println(test);
            //*
            for (Object temp : test.keyToIndexMap.keys()) {
                System.out.println(temp + " , " + test.keyToIndexMap.get(temp));
            }
            System.out.println(test.size);
        }
    }
    }
     //*/
}