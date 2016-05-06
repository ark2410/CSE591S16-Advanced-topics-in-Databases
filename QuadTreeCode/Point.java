public class Point {
	
	private double x;
    private double y;
    private Object value;

    // contains the x and y coordinate of the point in the universe and the value 
    // associated with that point
    public Point(double x, double y, int value) {
        this.x = x;
        this.y = y;
        this.value = value;
    }

    public double getX() {
        return x;
    }

    public void setX(double x) {
        this.x = x;
    }

    public double getY() {
        return y;
    }

    public void setY(double y) {
        this.y = y;
    }

    public Object getValue() {
        return value;
    }

    public void setValue(Object value) {
        this.value = value;
    }
}