
public class Node {

	private double xCordinate;
    private double yCordinate;
    private double width;
    private double height;
    private Node parent;
    private Point point;
    private NodeType nodetype = NodeType.EMPTY;
    private Node northWest;
    private Node northEast;
    private Node southWest;
    private Node southEast;

    // Makes new Quad Tree node
    public Node(double x, double y, double w, double h, Node parent) {
        this.xCordinate = x;
        this.yCordinate = y;
        this.width = w;
        this.height = h;
        this.parent = parent;
    }

    public double getxCordinate() {
        return xCordinate;
    }

    public void setxCordinate(double x) {
        this.xCordinate = x;
    }

    public double getyCordinate() {
        return yCordinate;
    }

    public void setyCordinate(double y) {
        this.yCordinate = y;
    }

    public double getWidth() {
        return width;
    }

    public void setWidth(double w) {
        this.width = w;
    }

    public double getHeight() {
        return height;
    }

    public void setHeight(double h) {
        this.height = h;
    }

    public Node getParent() {
        return parent;
    }

    public void setParent(Node opt_parent) {
        this.parent = opt_parent;
    }

    public void setPoint(Point point) {
        this.point = point;
    }

    public Point getPoint() {
        return this.point;
    }

	public void setNodeType(NodeType nodetype) {
        this.nodetype = nodetype;
    }

    public NodeType getNodeType() {
        return this.nodetype;
    }


    public void setNorthWest(Node nw) {
        this.northWest = nw;
    }

    public void setNorthEast(Node ne) {
        this.northEast = ne;
    }

    public void setSouthWest(Node sw) {
        this.southWest = sw;
    }

    public void setSouthEast(Node se) {
        this.southEast = se;
    }

    public Node getNorthEast() {
        return northEast;
    }

    public Node getNorthWest() {
        return northWest;
    }

    public Node getSouthWest() {
        return southWest;
    }

    public Node getSouthEast() {
        return southEast;
    }
}
