package src;

public class TreeNode {
    public String feature;
    public float threshold;
    public boolean isLeaf;
    public int prediction;
    public TreeNode left;
    public TreeNode right;

    public TreeNode() {
        this.isLeaf = false;
        this.left = null;
        this.right = null;
    }
} 