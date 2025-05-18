package src;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class StrokeRiskML {
    private List<UserProfile> trainingData;
    private List<UserProfile> testData;
    private RandomForest forest;
    private Map<String, Double> featureImportance;
    private double accuracy;
    private double precision;
    private double recall;
    private double f1Score;
    private int patientCounter;

    public StrokeRiskML() {
        this.trainingData = new ArrayList<>();
        this.testData = new ArrayList<>();
        this.forest = new RandomForest();
        this.featureImportance = new HashMap<>();
        this.patientCounter = 1;
    }

    public void loadAndPreprocessData(String csvPath) {
        try (BufferedReader br = new BufferedReader(new FileReader(csvPath))) {
            String header = br.readLine(); // Skip header
            List<String> lines = br.lines().collect(Collectors.toList());
            
            // Shuffle data for random split
            Collections.shuffle(lines);
            
            // Split into training (80%) and test (20%) sets
            int splitIndex = (int) (lines.size() * 0.8);
            List<String> trainingLines = lines.subList(0, splitIndex);
            List<String> testLines = lines.subList(splitIndex, lines.size());

            // Process training data
            for (String line : trainingLines) {
                UserProfile profile = parseCSVLine(line);
                if (profile != null) {
                    trainingData.add(profile);
                }
            }

            // Process test data
            for (String line : testLines) {
                UserProfile profile = parseCSVLine(line);
                if (profile != null) {
                    testData.add(profile);
                }
            }

            // Calculate feature importance based on training data
            calculateFeatureImportance();

            System.out.println("Loaded " + trainingData.size() + " training samples and " + testData.size() + " test samples");
        } catch (IOException e) {
            System.err.println("Error loading data: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private void calculateFeatureImportance() {
        // Initialize importance scores
        Map<String, Double> importance = new HashMap<>();
        String[] features = {
            "age", "HighBloodPressure", "ChestPain", "Dizziness", "Fatigue & Weakness",
            "Swelling (Edema)", "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating",
            "Chest Discomfort (Activity)", "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom"
        };

        for (String feature : features) {
            importance.put(feature, 0.0);
        }

        // Calculate importance based on correlation with stroke outcome
        for (UserProfile profile : trainingData) {
            boolean hadStroke = profile.healthIndicators.getOrDefault("stroke", false);
            
            // Age importance (normalized)
            double ageScore = profile.age / 100.0;
            importance.put("age", importance.get("age") + (hadStroke ? ageScore : -ageScore));

            // Other features importance
            for (String feature : features) {
                if (!feature.equals("age")) {
                    boolean hasFeature = profile.healthIndicators.getOrDefault(feature, false);
                    importance.put(feature, importance.get(feature) + (hadStroke && hasFeature ? 1.0 : -0.5));
                }
            }
        }

        // Normalize importance scores
        double maxImportance = importance.values().stream().mapToDouble(Math::abs).max().orElse(1.0);
        for (String feature : features) {
            featureImportance.put(feature, Math.abs(importance.get(feature)) / maxImportance);
        }
    }

    private UserProfile parseCSVLine(String line) {
        try {
            String[] values = line.split(",");
            if (values.length < 17) { // We need at least 17 columns based on the CSV structure
                System.err.println("Invalid line format: " + line);
                return null;
            }

            // Generate a simple patient ID in the format "P1", "P2", etc.
            String patientId = "P" + patientCounter++;
            
            // Parse age from the second-to-last column
            int age;
            try {
                age = Integer.parseInt(values[values.length - 3].trim());
                if (age < 0 || age > 120) {
                    System.err.println("Invalid age value: " + age);
                    return null;
                }
            } catch (NumberFormatException e) {
                System.err.println("Error parsing age: " + values[values.length - 3]);
                return null;
            }
            
            UserProfile profile = new UserProfile(patientId, age);

            // Add health indicators in the correct order
            String[] indicators = {
                "Chest Pain", "Shortness of Breath", "Irregular Heartbeat", 
                "Fatigue & Weakness", "Dizziness", "Swelling (Edema)",
                "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating",
                "Persistent Cough", "Nausea/Vomiting", "High Blood Pressure",
                "Chest Discomfort (Activity)", "Cold Hands/Feet",
                "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom"
            };

            // Parse health indicators from the first 15 columns
            for (int i = 0; i < indicators.length; i++) {
                String value = values[i].trim();
                boolean hasIndicator = value.equals("1"); // The CSV uses 1/0 instead of Yes/No
                profile.addHealthIndicator(indicators[i], hasIndicator);
            }

            // Add stroke outcome from the last column (At Risk Binary)
            boolean hadStroke = values[values.length - 1].trim().equals("1");
            profile.addHealthIndicator("stroke", hadStroke);

            return profile;
        } catch (Exception e) {
            System.err.println("Error parsing line: " + line);
            e.printStackTrace();
            return null;
        }
    }

    public void trainModel() {
        // Train with 100 trees and max depth of 10 for better accuracy
        forest.train(trainingData, 100, 10);
        evaluateModel();
    }

    private void evaluateModel() {
        int truePositives = 0;
        int falsePositives = 0;
        int trueNegatives = 0;
        int falseNegatives = 0;

        for (UserProfile profile : testData) {
            int prediction = forest.predict(profile);
            boolean actual = profile.healthIndicators.getOrDefault("stroke", false);

            if (prediction == 1 && actual) truePositives++;
            else if (prediction == 1 && !actual) falsePositives++;
            else if (prediction == 0 && !actual) trueNegatives++;
            else if (prediction == 0 && actual) falseNegatives++;
        }

        // Calculate metrics
        accuracy = (double) (truePositives + trueNegatives) / testData.size();
        precision = (double) truePositives / (truePositives + falsePositives);
        recall = (double) truePositives / (truePositives + falseNegatives);
        f1Score = 2 * (precision * recall) / (precision + recall);

        System.out.println("\nModel Performance Metrics:");
        System.out.printf("Accuracy: %.2f%%\n", accuracy * 100);
        System.out.printf("Precision: %.2f%%\n", precision * 100);
        System.out.printf("Recall: %.2f%%\n", recall * 100);
        System.out.printf("F1 Score: %.2f%%\n", f1Score * 100);
    }

    public String getDetailedPrediction(UserProfile user) {
        // Get binary prediction (1 or 0) from the Random Forest model
        int prediction = forest.predict(user);
        StringBuilder details = new StringBuilder();
        
        // Add prediction result
        details.append("Prediction: ").append(prediction == 1 ? "At Risk (1)" : "Not At Risk (0)").append("\n\n");
        
        // Add contributing factors
        details.append("Contributing Factors:\n");
        
        // Age factor
        if (user.age >= 70) {
            details.append(String.format("- Advanced age (70+): High impact\n"));
        } else if (user.age >= 50) {
            details.append(String.format("- Middle age (50-69): Moderate impact\n"));
        }
        
        // Health indicators
        for (Map.Entry<String, Boolean> entry : user.healthIndicators.entrySet()) {
            if (entry.getValue() && !entry.getKey().equals("stroke")) {
                double importance = featureImportance.getOrDefault(entry.getKey(), 0.0);
                if (importance > 0.7) {
                    details.append(String.format("- %s: High impact\n", entry.getKey()));
                } else if (importance > 0.4) {
                    details.append(String.format("- %s: Moderate impact\n", entry.getKey()));
                } else {
                    details.append(String.format("- %s: Low impact\n", entry.getKey()));
                }
            }
        }

        // Add model confidence
        float proba = forest.predictProba(user);
        details.append("\nModel Confidence: ").append(String.format("%.1f%%", proba * 100));
        if (proba == 0.0f || proba == 1.0f) {
            details.append("\n[Note: Model is very certain in its prediction]");
        }

        return details.toString();
    }

    public int predictRisk(UserProfile user) {
        // Use the Random Forest model to make the prediction
        return forest.predict(user);
    }

    public List<UserProfile> getTrainingData() {
        return trainingData;
    }
} 