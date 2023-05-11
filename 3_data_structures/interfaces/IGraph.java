package edu.caltech.cs2.interfaces;

public interface IGraph<V, E> {
    /**
     * Add a vertex to the graph.
     * @param vertex The vertex to add
     * @return true if vertex was not present already.
     */
    public boolean addVertex(V vertex);

    /**
     * Adds edge e to the graph.
     *
     * @param e The edge to add.
     * @throws IllegalArgumentException
     *             If src and dest are not valid vertices (eg. refers to vertices not in the graph).
     * @return true if an edge from src to dest did not exist previously.
     *         false if an edge from src to dest did exist previously, in which case the edge value is updated
     */
    public boolean addEdge(V src, V dest, E e);

    /**
     * Adds edge e to the graph in both directionns.
     *
     * @param e The edge to add.
     * @throws IllegalArgumentException
     *    If v1 and v2 are not valid vertices (eg. refers to vertices not in the graph).
     * @return true if an edge between src and dest did not exist in either direction.
     *         false if an edge between src and dest existed in either direction, in which case the edge value is
     *         updated.
     */
    public boolean addUndirectedEdge(V v1, V v2, E e);

    /**
     * Remove an edge from src to dest from the graph.
     *
     * @throws IllegalArgumentException if src or dest is not in the graph.
     * @return true if an edge from src to dest was present.
     */
    public boolean removeEdge(V src, V dest);

    /**
     * Returns the set of vertices in the graph.
     * @return The set of all vertices in the graph.
     */
    public ISet<V> vertices();

    /**
     * Tests if vertices i and j are adjacent, returning the edge between
     * them if so.
     *
     * @throws IllegalArgumentException if i or j are not vertices in the graph.
     * @return The edge from i to j if it exists in the graph;
     * 		   null otherwise.
     */
    public E adjacent(V i, V j);

    /**
     * Return the neighbours of a given vertex when this graph is treated as
     * DIRECTED; that is, vertices to which vertex has an outgoing edge.
     *
     * @param vertex The vertex the neighbours of which to return.
     * @throws IllegalArgumentException if vertex is not in the graph.
     * @return The set of neighbors of vertex.
     */
    public ISet<V> neighbors(V vertex);
}