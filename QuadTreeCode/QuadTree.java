import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

public class QuadTree {
	
	private Node root;
	
	public QuadTree(double minX, double minY, double maxX, double maxY) {
        this.root = new Node(minX, minY, maxX - minX, maxY - minY, null);
    }
	
	public Node getRootNode() {
        return this.root;
    }
	
	public void putInTree(double x, double y, int value) throws Exception {

        Node root = this.root;
        if (x < root.getxCordinate() || y < root.getyCordinate() || x > root.getxCordinate() + root.getWidth() || y > root.getyCordinate() + root.getHeight()) {
            throw new Exception("Out of bounds : (" + x + ", " + y + ")");
        }
        else{
        	this.insert(root, new Point(x, y, value));
        }
        maintainUniverse(x, y, value);
    }
	
	private void insert(Node parent, Point point) throws Exception {
        switch (parent.getNodeType()) {
            case EMPTY:
                this.addPoint(parent, point);
                break;
            case LEAF:
                if (parent.getPoint().getX() == point.getX() && parent.getPoint().getY() == point.getY()) {
                    this.addPoint(parent, point);
                } else {
                    this.refactorNode(parent);
                    this.insert(parent, point);
                }
                break;
            case POINTER:
                this.insert(this.getQuadrant(parent, point.getX(), point.getY()), point);
                break;

            default:
                throw new Exception("No such Node Exists");
        }
    }
	
    private void addPoint(Node node, Point point) throws Exception {
        if (node.getNodeType() == NodeType.POINTER) {
            throw new Exception("The node is not a LEAf node");
        }
        node.setNodeType(NodeType.LEAF);
        node.setPoint(point);
    }
    
    private Node getQuadrant(Node parent, double xCordinate, double yCordinate) {
        double calculatedX = parent.getxCordinate() + parent.getWidth() / 2;
        double calculatedy = parent.getyCordinate() + parent.getHeight() / 2;
        if (xCordinate < calculatedX) {
        	if(yCordinate < calculatedy){
        		return parent.getNorthWest();
        	}
        	else {
        		return parent.getSouthWest();
        	}
        } else {
        	if(yCordinate < calculatedy){
        		return parent.getNorthEast();
        	}
        	else {
        		return parent.getSouthEast();
        	}
        }
    }
    
    private void refactorNode(Node node) throws Exception {
        Point current = node.getPoint();
        node.setPoint(null);
        node.setNodeType(NodeType.POINTER);
        double newX = node.getxCordinate();
        double newY = node.getyCordinate();
        double newWidth = node.getWidth() / 2;
        double newheight = node.getHeight() / 2;
        node.setNorthWest(new Node(newX, newY, newWidth, newheight, node));
        node.setNorthEast(new Node(newX + newWidth, newY, newWidth, newheight, node));
        node.setSouthWest(new Node(newX, newY + newheight, newWidth, newheight, node));
        node.setSouthEast(new Node(newX + newWidth, newY + newheight, newWidth, newheight, node));
        this.insert(node, current);
    }
    
    public boolean remove(double x, double y) throws Exception {
        Node node = this.find(this.root, x, y);
        if (node != null) {
            node.setPoint(null);
            node.setNodeType(NodeType.EMPTY);
            this.balance(node);
            return true;
        } else {
            return false;
        }
    }
    
    public Node find(Node node, double x, double y) throws Exception {
        Node resultLeaf = null;
        switch (node.getNodeType()) {
            case EMPTY:
                break;
            case LEAF:
            	if(node.getPoint().getX() == x && node.getPoint().getY() == y){
            		resultLeaf = node;
            	}
            	else{
            		resultLeaf = null;
            	}
            	break;
            case POINTER:
            	resultLeaf = this.find(this.getQuadrant(node, x, y), x, y);
                break;
            default:
                throw new Exception("No such node exists");
        }
        return resultLeaf;
    }

    private void balance(Node node) {
        switch (node.getNodeType()) {
            case EMPTY:
            case LEAF:
                if (node.getParent() != null) {
                    this.balance(node.getParent());
                }
                break;
            case POINTER: {
                Node northWest = node.getNorthWest();
                Node northEast = node.getNorthEast();
                Node southWest = node.getSouthWest();
                Node southEast = node.getSouthEast();
                Node place = null;
                if (northWest.getNodeType() != NodeType.EMPTY) {
                	place = northWest;
                }
                if (northEast.getNodeType() != NodeType.EMPTY) {
                    if (place != null) {
                        break;
                    }
                    place = northEast;
                }
                if (southWest.getNodeType() != NodeType.EMPTY) {
                    if (place != null) {
                        break;
                    }
                    place = southWest;
                }
                if (southEast.getNodeType() != NodeType.EMPTY) {
                    if (place != null) {
                        break;
                    }
                    place = southEast;
                }
                if (place == null) {
                    node.setNodeType(NodeType.EMPTY);
                    node.setNorthWest(null);
                    node.setNorthEast(null);
                    node.setSouthWest(null);
                    node.setSouthEast(null);
                } else if (place.getNodeType() == NodeType.POINTER) {
                    break;
                } else {
                    node.setNodeType(NodeType.LEAF);
                    node.setNorthWest(null);
                    node.setNorthEast(null);
                    node.setSouthWest(null);
                    node.setSouthEast(null);
                    node.setPoint(place.getPoint());
                }
                if (node.getParent() != null) {
                    this.balance(node.getParent());
                }
            }
            break;
        }
    }
    
    final List<Point> arrIntersect = new ArrayList<Point>();
    final List<Point> arrWithin = new ArrayList<Point>();
    List<Point> universe = new ArrayList<Point>();
    
    public Point[] maintainUniverse(double x, double y, int i){
    	Point p1 = new Point(x,y,i);
    	universe.add(p1);
    	return null;
    }
    
    public int[] KNN(double x1, double y1, double x2, double y2,int k){
    	double centroidX = (Math.abs(x2-x1))/2;
    	double centroidY = (Math.abs(y2-y1))/2;
    	List<Integer> temp = new ArrayList<Integer>();
    	int[] result= new int[k];
		HashMap<Point,Double> hm = new HashMap<Point,Double>();
		for(int i=0; i < universe.size();i++ ){
			double a = Math.pow(Math.abs(centroidX-universe.get(i).getX()), 2);
			double b = Math.pow(Math.abs(centroidY-universe.get(i).getY()), 2);
			double dist = Math.sqrt(Math.abs(a+b));
			hm.put(universe.get(i), dist);
		}
		Set<Entry<Point, Double>> set = hm.entrySet();
		List<Entry<Point, Double>> list = new ArrayList<Entry<Point, Double>>(set);
		Collections.sort(list, new Comparator<Map.Entry<Point, Double>>() {
			@Override
			public int compare(Entry<Point, Double> o1, Entry<Point, Double> o2) {
				return o2.getValue().compareTo(o1.getValue());
			}
        });
		for(int i=0; i<k*4;i++){
			temp.add((Integer) list.get(i).getKey().getValue());
		}
		Set<Integer> hs = new HashSet<>();
		hs.addAll(temp);
		temp.clear();
		temp.addAll(hs);
		for(int i =0; i < k;i++ ){
			result[i] = temp.get(i);
		}
		return result;
    } 
    
    
    
    
    public Point[] intersect(final double xmin, final double ymin, final double xmax, final double ymax) {
        
        this.decideIntersect(this.root, xmin, ymin, xmax, ymax);
        return arrIntersect.toArray(new Point[arrIntersect.size()]);
    }

    
    public void helpIntersect(Node node, final double xmin, final double ymin, final double xmax, final double ymax) {
        Point point = node.getPoint();
        if (point.getX() < xmin || point.getX() > xmax || point.getY() < ymin || point.getY() > ymax) {
            //System.out.println("not inside");
        } else {
            arrIntersect.add(node.getPoint());
        }

    }
    
  
    public void decideIntersect(Node node, double xmin, double ymin, double xmax, double ymax) {
        switch (node.getNodeType()) {
            case LEAF:
                helpIntersect(node,xmin,ymin,xmax,ymax);
                break;

            case POINTER:
                if (checkIntersection(xmin, ymax, xmax, ymin, node.getNorthEast()))
                    this.decideIntersect(node.getNorthEast(), xmin, ymin, xmax, ymax);
                if (checkIntersection(xmin, ymax, xmax, ymin, node.getSouthEast()))
                    this.decideIntersect(node.getSouthEast(), xmin, ymin, xmax, ymax);
                if (checkIntersection(xmin, ymax, xmax, ymin, node.getSouthWest()))
                    this.decideIntersect(node.getSouthWest(), xmin, ymin, xmax, ymax);
                if (checkIntersection(xmin, ymax, xmax, ymin, node.getNorthWest()))
                    this.decideIntersect(node.getNorthWest(), xmin, ymin, xmax, ymax);
                break;
		default:
			break;
        }
    }
    
  
    public Point[] containment(final double xmin, final double ymin, final double xmax, final double ymax) {
        this.decideContainment(this.root, xmin, ymin, xmax, ymax);
        return arrWithin.toArray(new Point[arrWithin.size()]);
    }
    
  
    public void helpContainment(Node node, final double xmin, final double ymin, final double xmax, final double ymax) {
        Point pt = node.getPoint();
        if (pt.getX() > xmin && pt.getX() < xmax && pt.getY() > ymin && pt.getY() < ymax) {
            arrWithin.add(node.getPoint());
        }
    }
   
   
    public void decideContainment(Node node, double xmin, double ymin, double xmax, double ymax) {
        switch (node.getNodeType()) {
            case LEAF:
                helpContainment(node,xmin,ymin,xmax,ymax);
                break;

            case POINTER:
                if (checkIntersection(xmin, ymax, xmax, ymin, node.getNorthEast()))
                    this.decideContainment(node.getNorthEast(), xmin, ymin, xmax, ymax);
                if (checkIntersection(xmin, ymax, xmax, ymin, node.getSouthEast()))
                    this.decideContainment(node.getSouthEast(), xmin, ymin, xmax, ymax);
                if (checkIntersection(xmin, ymax, xmax, ymin, node.getSouthWest()))
                    this.decideContainment(node.getSouthWest(), xmin, ymin, xmax, ymax);
                if (checkIntersection(xmin, ymax, xmax, ymin, node.getNorthWest()))
                    this.decideContainment(node.getNorthWest(), xmin, ymin, xmax, ymax);
                break;
		default:
			break;
        }
    }
    
    private boolean checkIntersection(double left, double bottom, double right, double top, Node node) {
        return !(node.getxCordinate() > right ||
                (node.getxCordinate() + node.getWidth()) < left ||
                node.getyCordinate() > bottom ||
                (node.getyCordinate() + node.getHeight()) < top);
    }

    private QuadTree getTree() throws Exception{
    	return this;
    }
    
    public static void main(String args[]) throws Exception{
    	long startTime = System.currentTimeMillis();
    	QuadTree qt = new QuadTree(-999999999,-999999999,999999999,999999999);
    	BufferedReader br = null;
    	
    	try {

			String sCurrentLine;

			br = new BufferedReader(new FileReader("C:\\Users\\aman\\Desktop\\100krect.txt"));

			while ((sCurrentLine = br.readLine()) != null) {
				String[] splitInput = sCurrentLine.split(",");
				qt.putInTree(Double.parseDouble(splitInput[1]), 
						     Double.parseDouble(splitInput[2]), 
						     Integer.parseInt(splitInput[0]));
				qt.putInTree(Double.parseDouble(splitInput[3]), 
					     	 Double.parseDouble(splitInput[4]), 
					         Integer.parseInt(splitInput[0]));
				qt.putInTree(Double.parseDouble(splitInput[1]), 
					         Double.parseDouble(splitInput[4]), 
					         Integer.parseInt(splitInput[0]));
				qt.putInTree(Double.parseDouble(splitInput[3]), 
					         Double.parseDouble(splitInput[2]), 
					         Integer.parseInt(splitInput[0]));
			}

		} catch (IOException e) {
			e.printStackTrace();
		}
    	long endTime = System.currentTimeMillis();
    	long duration = endTime-startTime;
    	System.out.println("execution time insertion is: "+duration);
    	
    	
    	//Intersect test 2
    	long startTimemili1 = System.currentTimeMillis();
    	long startTimenano1 = System.nanoTime();
        QuadTree qt1 = qt.getTree();
        System.out.println("Intersection test: ");
    	Point[] points1 = qt1.intersect(-104.046822,45.559261,-101.023819,46.660042);
    	List<Object> qualifiedId = new ArrayList<Object>();
    	for(int i =0 ; i <points1.length;i++){
    		if(!qualifiedId.contains(points1[i].getValue())){
    			qualifiedId.add(points1[i].getValue());
    		}
    	}
    	for(int i = 0; i < qualifiedId.size();i++){
    		//System.out.println(qualifiedId.get(i));
    	}
    	//System.out.println("number of points: "+qualifiedId.size());
    	long endTimemili1 = System.currentTimeMillis();
    	long endTimenano1 = System.nanoTime();
    	long durationMili1 = endTimemili1-startTimemili1 ;
    	long durationNano1 = endTimenano1 - startTimenano1 ;
    	System.out.println("execution time intersection is: "+durationMili1+" mili seconds");
    	System.out.println("execution time intersection is: "+durationNano1+" nano seconds");
    	
    	
    	
        /*
    	// KNN query
        System.out.println("KNN Query test: ");
    	int[] KNNresult = qt1.KNN(0, 0, 10, 10, 10);
    	for(int i=0;i<KNNresult.length;i++){
    		System.out.println(KNNresult[i]);
    	}
    	*/
    	
    	
    	/*
        // Containment test
    	long startTimeMiliC = System.currentTimeMillis();
    	long startTimeNanoC = System.nanoTime();
    	QuadTree qt1 = qt.getTree();
        System.out.println("Containment test: ");
    	Point[] points2 = qt1.intersect(-104.046822,45.559261,-101.023819,46.660042);
    	List<Object> ObjectList = new ArrayList<Object>();
    	List<Object> ObjectList1 = new ArrayList<Object>();
    	for(int i =0 ; i <points2.length;i++){
    		ObjectList.add(points2[i].getValue());
    	}
    	for(int i=0; i<ObjectList.size();i++){
    		int occurrences = Collections.frequency(ObjectList,ObjectList.get(i) );
    		if((occurrences==4)&& !(ObjectList1.contains(ObjectList.get(i))) ){
    			ObjectList1.add(ObjectList.get(i));
    		}
    	}
    	for(int i=0; i < ObjectList1.size(); i++  ){
    		//System.out.println(ObjectList1.get(i));
    	}
    	//System.out.println("number of points: "+ObjectList1.size());
    	long endTimeMiliC = System.currentTimeMillis();
    	long endTimeNanoC = System.nanoTime();
    	long durationMiliC = endTimeMiliC-startTimeMiliC;
    	long durationNanoC = endTimeNanoC-startTimeNanoC;
    	System.out.println("execution time containment is: "+durationMiliC+" mili seconds");
    	System.out.println("execution time is: "+durationNanoC+" nano seconds");
    	*/
    	
    }
}

