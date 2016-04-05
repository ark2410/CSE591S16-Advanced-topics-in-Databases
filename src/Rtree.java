import java.util.LinkedList;

public class Rtree<T> {
	int maxFill;
	int minFill;
	int dimension;
	private Node root; 
	LinkedList<T> results = new LinkedList<T>();
	
	public Rtree(int maxFill, int minFill, int dimension){
		this.maxFill = maxFill;
		this.minFill = minFill;
		this.dimension = dimension;
	}
	
	private Node makeRoot(boolean isLeaf){
	{
	    float[] initCoords = new float[dimension];
	    float[] initDimensions = new float[dimension];
	    for (int i = 0; i < this.dimension; i++)
	    {
	      initCoords[i] = (float) Math.sqrt(Float.MAX_VALUE);
	      initDimensions[i] = -2.0f * (float) Math.sqrt(Float.MAX_VALUE);
	    }
	    return new Node(initCoords, initDimensions, isLeaf);
	  }
	}
	private void containment(float[] coords, float[] dimensions, Node node){
		if(node.leaf)
		{
			for(Node n: node.children){
				if (isOverlap(coords, dimensions, n.coords, n.dimensions))
		        {
		          results.add(((Item)n).item);
		        }
			}
		}
		else{
			for (Node n : node.children)
		      {
		        if (isOverlap(coords, dimensions, n.coords, n.dimensions))
		        {
		        	containment(coords, dimensions, n);
		        }
		      }
		}
	}
	
	private boolean isOverlap(float[] scoords, float[] sdimensions, float[] coords, float[] dimensions)
		  {
		    for (int i = 0; i < scoords.length; i++)
		    {
		      boolean overlapInThisDimension = false;
		      if (scoords[i] == coords[i])
		      {
		        overlapInThisDimension = true;
		      }
		      else if (scoords[i] < coords[i])
		      {
		        if (scoords[i] + sdimensions[i] >= coords[i])
		        {
		          overlapInThisDimension = true;
		        }
		      }
		      else if (scoords[i] > coords[i])
		      {
		        if (coords[i] + dimensions[i] >= scoords[i])
		        {
		          overlapInThisDimension = true;
		        }
		      }
		      if (!overlapInThisDimension)
		      {
		        return false;
		      }
		    }
		    return true;
		  }
	
	private class Item extends Node
	  {
	    final T item;

	    public Item(float[] coords, float[] dimensions, T item)
	    {
	      super(coords, dimensions, true);
	      this.item = item;
	    }

	    public String toString()
	    {
	      return "Item: " + item;
	    }
	  } 
	
}
