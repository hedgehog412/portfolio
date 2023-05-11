package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.IDeque;
import edu.caltech.cs2.interfaces.IQueue;
import edu.caltech.cs2.interfaces.IStack;

import java.util.Iterator;

public class LinkedDeque<E> implements IDeque<E>, IQueue<E>, IStack<E> {
    private static class Node<E>{
        public final E DATA;
        public Node<E> next;
        public Node<E> prev;
        public Node(E data){
            this(data, null, null);
        }
        public Node(E data, Node prev, Node next){
            this.DATA = data;
            this.prev = prev;
            this.next = next;
        }
    }
    private Node<E> head;
    private int size;
    private Node<E> tail;
    public LinkedDeque(){
        this.head = null;
        this.tail = null;
        this.size =0;
    }

    @Override
    public void addFront(E e) {
        if(this.head == null){
            this.head = new Node<E>(e);
            this.tail = this.head;
        }
        else{
            Node<E> newNode = new Node<E>(e,null,this.head);
            this.head.prev = newNode;
            this.head = newNode;
        }
        size ++;
    }

    @Override
    public void addBack(E e) {
        if(this.head == null){
            this.head = new Node<E>(e);
            this.tail = this.head;
        }
        else{
            Node<E> back = new Node<E>(e,this.tail,null);
            this.tail.next = back;
            this.tail = back;
        }
        size++;
    }

    @Override
    public E removeFront() {
        if(this.head == null){
            return null;
        }
        else if(this.head.next == null){
            Node<E> front = this.head;
            this.head = null;
            this.tail = null;
            size--;
            return front.DATA;
        }
        else{
            Node<E> front = this.head;
            this.head = this.head.next;
            this.head.prev = null;
            size--;
            return front.DATA;
        }

    }

    @Override
    public E removeBack() {
        if(this.head ==null){
            return null;
        }
        else if(this.head.next == null){
            Node<E> back = head;
            this.head = null;
            this.tail = null;
            size --;
            return back.DATA;
        }
        else{
            Node<E> back = this.tail;
            this.tail = this.tail.prev;
            this.tail.next = null;
            size--;
            return back.DATA;
        }
    }

    @Override
    public boolean enqueue(E e) {
        addBack(e);
        return true;
    }

    @Override
    public E dequeue() {
        return removeFront();
    }

    @Override
    public boolean push(E e) {
        addFront(e);
        return true;
    }

    @Override
    public E pop() {
        return removeFront();
    }

    @Override
    public E peekFront() {
        if(this.head == null){
            return null;
        }
        return head.DATA;
    }

    @Override
    public E peekBack() {
        if(this.tail == null){
            return null;
        }
        return tail.DATA;
    }

    @Override
    public E peek() {
        return peekFront();
    }

    @Override
    public Iterator<E> iterator() {
        return new LinkedDequeIterator();
    }
    private class LinkedDequeIterator implements Iterator<E>{
        private Node curr;
        private int i = 0;

        public LinkedDequeIterator(){
            this.curr = (LinkedDeque.this).head;
        }
        @Override
        public boolean hasNext() {
            return i<(LinkedDeque.this).size();
        }

        @Override
        public E next() {
            i++;
            E data = (E) curr.DATA;
            curr = curr.next;
            return data;
        }
    }

    public int size(){
        return size;
    }

    public String toString(){
        if(this.size == 0){
            return "[]";
        }
        else{
            String result = "[";
            Node<E> curr = this.head;
            while(curr != null){
                result += curr.DATA + ", ";
                curr = curr.next;
            }
            return result.substring(0,result.length()-2) + "]";
        }
    }

}
