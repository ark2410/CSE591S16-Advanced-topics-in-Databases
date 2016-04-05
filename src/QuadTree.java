import QuadtreeNode;

public class QuadTree {
	 private QuadtreeNode root;
	 private int count = 0;
	 
	 public QuadTree(double minX, double minY, double maxX, double maxY) 
	 {
		 this.root = new QuadtreeNode(minX, minY, maxX - minX, maxY - minY, null);
	 }
	 
	 public QuadtreeNode getRootNode() {
		 return this.root;
	 }
	 
	 public void set(double x, double y, Object value) throws Exception {

		 QuadtreeNode root = this.root;
        if (x < root.getX() || y < root.getY() || x > root.getX() + root.getW() || y > root.getY() + root.getH()) {
            throw new Exception("Out of bounds : (" + x + ", " + y + ")");
        }
        if (this.insert(root, new Point(x, y, value))) {
            this.count++;
        }
     }
	 
	 private boolean insert(QuadtreeNode parent, Point point) {
	        Boolean result = false;
	        switch(parent.getNodetype())
	        {
		        case EMPTY:
		        	this.setPointForNode(parent, point);
	                result = true;
	                break;
		        case LEAF:
		        	if (parent.getPoint().getX() == point.getX() && parent.getPoint().getY() == point.getY()) {
	                    this.setPointForNode(parent, point);
	                    result = false;
	                } else {
	                    this.split(parent);
	                    result = this.insert(parent, point);
	                }
	                break;
		        case POINTER:	
		        	result = this.insert(
	                        this.getQuadrantForPoint(parent, point.getX(), point.getY()), point);
	                break;
		        default:
	                throw new Exception("Invalid nodeType in parent");
	        }
	        return result;
	    }
	 
	 private void setPointForNode(QuadtreeNode node, Point point) 
	 {
         if (node.getNodetype() == NodeType.POINTER) {
             throw new Exception("Can not set point for node of type POINTER");
	         }
	         node.setNodeType(NodeType.LEAF);
	         node.setPoint(point);
	 }
	 
	 private QuadtreeNode getQuadrantForPoint(QuadtreeNode parent, double x, double y) {
		        double mx = parent.getX() + parent.getW() / 2;
		         double my = parent.getY() + parent.getH() / 2;
		         if (x < mx) {
		             return y < my ? parent.getNw() : parent.getSw();
		         } else {
		             return y < my ? parent.getNe() : parent.getSe();
		         }
		     }
	 
 
}
