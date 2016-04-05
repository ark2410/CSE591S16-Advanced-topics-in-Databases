
public class Point implements Comparable{

	private double x;
	private double y;
	private Object value;
	
	public Point(double x, double y, Object value) {
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

	public int compareTo(Object o) {
		Point tmp = (Point) o;
		if (this.x < tmp.x) {
			return -1;
		}
		else if (this.x > tmp.x) {
			return 1;
		}
		return 0;
	}

}
