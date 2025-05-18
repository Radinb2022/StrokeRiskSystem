package src;

import java.util.*;

public class RandomForest {
    private List<TreeNode> trees;
    private static final Random random = new Random();

    public RandomForest() {
        this.trees = new ArrayList<>();
    }

    private static float randFloat(float min, float max) {
        return min + random.nextFloat() * (max - min);
    }

    private TreeNode buildTree(List<UserProfile> data, int depth, int maxDepth) {
        if (data.isEmpty() || depth >= maxDepth) {
            int atRisk = 0;
            for (UserProfile u : data) {
                if (u.healthIndicators.getOrDefault("stroke", false)) {
                    atRisk++;
                }
            }
            TreeNode leaf = new TreeNode();
            leaf.isLeaf = true;
            leaf.prediction = (atRisk > data.size() * 0.3) ? 1 : 0;
            return leaf;
        }

        String[] features = {
            "Chest Pain", "Shortness of Breath", "Irregular Heartbeat", 
            "Fatigue & Weakness", "Dizziness", "Swelling (Edema)",
            "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating",
            "Persistent Cough", "Nausea/Vomiting", "High Blood Pressure",
            "Chest Discomfort (Activity)", "Cold Hands/Feet",
            "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom", "Age"
        };
        String feature = features[random.nextInt(features.length)];
        float threshold = 0;

        if (feature.equals("Age")) {
            int minAge = Integer.MAX_VALUE;
            int maxAge = Integer.MIN_VALUE;
            for (UserProfile u : data) {
                minAge = Math.min(minAge, u.age);
                maxAge = Math.max(maxAge, u.age);
            }
            threshold = randFloat(minAge, maxAge);
        }

        List<UserProfile> left = new ArrayList<>();
        List<UserProfile> right = new ArrayList<>();

        for (UserProfile u : data) {
            if (feature.equals("Age")) {
                if (u.age < threshold) {
                    left.add(u);
                } else {
                    right.add(u);
                }
            } else {
                if (u.healthIndicators.getOrDefault(feature, false)) {
                    left.add(u);
                } else {
                    right.add(u);
                }
            }
        }

        TreeNode node = new TreeNode();
        node.feature = feature;
        node.threshold = threshold;
        node.isLeaf = false;
        node.left = buildTree(left, depth + 1, maxDepth);
        node.right = buildTree(right, depth + 1, maxDepth);
        return node;
    }

    public void train(List<UserProfile> data, int nTrees, int maxDepth) {
        trees.clear();
        for (int i = 0; i < nTrees; i++) {
            List<UserProfile> sample = new ArrayList<>();
            List<UserProfile> atRisk = new ArrayList<>();
            List<UserProfile> notAtRisk = new ArrayList<>();
            
            for (UserProfile u : data) {
                if (u.healthIndicators.getOrDefault("stroke", false)) {
                    atRisk.add(u);
                } else {
                    notAtRisk.add(u);
                }
            }
            
            int sampleSize = Math.min(atRisk.size(), notAtRisk.size());
            for (int j = 0; j < sampleSize; j++) {
                sample.add(atRisk.get(random.nextInt(atRisk.size())));
                sample.add(notAtRisk.get(random.nextInt(notAtRisk.size())));
            }
            
            trees.add(buildTree(sample, 0, maxDepth));
        }
    }

    private int predictTree(TreeNode node, UserProfile user) {
        if (node == null) return 0;
        if (node.isLeaf) return node.prediction;
        if (node.feature.equals("Age")) {
            if (user.age < node.threshold) {
                return predictTree(node.left, user);
            } else {
                return predictTree(node.right, user);
            }
        } else {
            if (user.healthIndicators.getOrDefault(node.feature, false)) {
                return predictTree(node.left, user);
            } else {
                return predictTree(node.right, user);
            }
        }
    }

    public int predict(UserProfile user) {
        int votes = 0;
        for (TreeNode t : trees) {
            votes += predictTree(t, user);
        }
        return (votes > trees.size() * 0.4) ? 1 : 0;
    }

    public float predictProba(UserProfile user) {
        int votes = 0;
        for (TreeNode t : trees) {
            votes += predictTree(t, user);
        }
        return (float) votes / trees.size();
    }
} 