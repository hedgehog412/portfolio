package edu.caltech.cs2.datastructures;

import edu.caltech.cs2.interfaces.IGraph;
import edu.caltech.cs2.interfaces.ISet;

public class Graph<V, E> implements IGraph<V, E> {
    ChainingHashDictionary<V, ChainingHashDictionary> graph;

    public Graph(){
        this.graph = new ChainingHashDictionary(MoveToFrontDictionary::new);
    }

    @Override
    public boolean addVertex(V vertex) {
        if(this.graph.containsKey(vertex)){
            return false;
        }
        this.graph.put(vertex, new ChainingHashDictionary(MoveToFrontDictionary::new));
        return true;
    }

    @Override
    public boolean addEdge(V src, V dest, E e) {
        if(!this.graph.containsKey(src)||!this.graph.containsKey(dest)) {
            throw new IllegalArgumentException();
        }
        if(this.graph.get(src).put(dest, e)!=null){
            return false;
        }
        return true;
    }

    @Override
    public boolean addUndirectedEdge(V n1, V n2, E e) {
        if(!this.graph.containsKey(n1)||!this.graph.containsKey(n2)) {
            throw new IllegalArgumentException();
        }
        boolean result=true;
        if(this.graph.get(n1).put(n2, e)!=null){
            result = false;
        }
        if(this.graph.get(n2).put(n1,e)!=null){
            result = false;
        }
        return result;
    }

    @Override
    public boolean removeEdge(V src, V dest) {
        if(!this.graph.containsKey(src)||!this.graph.containsKey(dest)) {
            throw new IllegalArgumentException();
        }
        if(this.graph.get(src).remove(dest)!=null){
            return true;
        }
        return false;
    }

    @Override
    public ISet<V> vertices() {
        return this.graph.keySet();
    }

    @Override
    public E adjacent(V i, V j) {
        if(!this.graph.containsKey(i)||!this.graph.containsKey(j)) {
            throw new IllegalArgumentException();
        }
        E edge = (E) this.graph.get(i).get(j);
        return edge;
    }

    @Override
    public ISet<V> neighbors(V vertex) {
        return this.graph.get(vertex).keySet();
    }
}