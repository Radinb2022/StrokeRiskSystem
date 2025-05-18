package src;

import java.util.List;

public class StrokeRiskPredictor {
    private RandomForest forest;

    public StrokeRiskPredictor() {
        this.forest = new RandomForest();
    }

    public void train(List<UserProfile> data, int nTrees, int maxDepth) {
        forest.train(data, nTrees, maxDepth);
    }

    public int predict(UserProfile user) {
        return forest.predict(user);
    }

    public float predictProba(UserProfile user) {
        return forest.predictProba(user);
    }

    public float calculateRisk(UserProfile user) {
        float score = 0;
        if (user.age >= 70) score += 30;
        if (user.healthIndicators.getOrDefault("HighBloodPressure", false)) score += 25;
        if (user.healthIndicators.getOrDefault("ChestPain", false)) score += 20;
        if (user.healthIndicators.getOrDefault("Dizziness", false)) score += 10;
        return Math.min(score, 100.0f);
    }

    public String generateAlert(float risk) {
        return (risk >= 50) ? "At Risk" : "Not At Risk";
    }

    public void addTrainingData(UserProfile user, KaggleData data) {
        data.appendUser(user);
        train(data.getDataset(), 10, 5); // Default values: 10 trees, max depth 5
    }
} 