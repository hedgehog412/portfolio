package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.IDeque;
import edu.caltech.cs2.interfaces.IQueue;
import edu.caltech.cs2.interfaces.IStack;

import java.util.Iterator;

public class ArrayDeque<E> implements IDeque<E>, IQueue<E>, IStack<E> {
    private E[] data;
    private int size;
    private static final int DEFAULT_CAPACITY = 10;
    private static final int GROW_FACTOR = 2;
    public ArrayDeque(){
        this(DEFAULT_CAPACITY);
    }

    public ArrayDeque(int initialCapacity){
        this.size = 0;
        this.data = (E[])new Object[initialCapacity];
    }

    private void resize(){
        if(size == this.data.length){
            E[] newData =  (E[]) new Object[this.data.length*GROW_FACTOR];
            for(int i =0;i<this.data.length;i++){
                newData[i] = data[i];
            }
            data = newData;
        }
    }
    @Override
    public void addFront(E e) {
        resize();
        if (this.size != 0){
            for (int i = this.size; i > 0; i--) {
                data[i] = data[i - 1];
            }
        }
        data[0] = e;
        size++;
    }

    @Override
    public void addBack(E e) {
        resize();
        data[size] = e;
        size++;
    }

    @Override
    public E removeFront() {
        E front;
        if(this.size > 0){
            front = this.data[0];
            for(int i = 0;i<this.size-1;i++){
                this.data[i] = this.data[i+1];
            }
            size--;
        }
        else{
            front = null;
        }
        return front;
    }

    @Override
    public E removeBack() {
        E back;
        if(this.size>0){
            back = this.data[this.size - 1];
            size--;
        }
        else{
            back = null;
        }
        return back;
    }

    @Override
    public boolean enqueue(E e) {
        addFront(e);
        return true;
    }

    @Override
    public E dequeue() {
        return removeBack();
    }

    @Override
    public boolean push(E e) {
        addBack(e);
        return true;
    }

    @Override
    public E pop() {
        return removeBack();
    }

    @Override
    public E peekFront() {
        E front;
        if(this.size>0){
            front = this.data[0];
        }
        else{
            front = null;
        }
        return front;
    }

    @Override
    public E peekBack() {
        E back;
        if(this.size>0){
            back = this.data[this.size -1];
        }
        else{
            back = null;
        }
        return back;
    }

    @Override
    public E peek() {
        return peekBack();
    }

    @Override
    public Iterator<E> iterator() {
        return new ArrayDequeIterator();
    }

    private class ArrayDequeIterator implements Iterator<E>{
        private int currentIdx;
        ArrayDequeIterator(){
            this.currentIdx = 0;
        }

        @Override
        public boolean hasNext() {
            return this.currentIdx<(ArrayDeque.this).size();
        }

        public E next(){
            E element = (ArrayDeque.this).data[currentIdx];
            this.currentIdx++;
            return element;
        }
    }

    @Override
    public int size() {
        return this.size;
    }

    public String toString(){
        if(this.size == 0){
            return "[]";
        }
        else{
            String result = "[";
            for (int i = 0;i<size;i++){
                result += this.data[i] + ", ";
            }
            result = result.substring(0, result.length() - 2);
            return result + "]";
        }
    }
    private void ensureCapacity() {
        if (this.size >= this.data.length) {
            // Create a new array of double size
            E[] newData = (E[])new Object[this.size * 2];
            // Copy the contents from old to new
            for (int i = 0; i < this.size; i++) {
                newData[i] = this.data[i];
            }
            // Set the field to new array
            this.data = newData;
        }
    }

}

