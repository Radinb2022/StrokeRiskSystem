package src;

public class Main {
    public static void main(String[] args) {
        System.out.println("\n=== Testing Random Forest Functions ===\n");
        
        // Create instances for testing
        RandomForest forest = new RandomForest();
        KaggleData data = new KaggleData();
        data.loadSampleData();
        
        // Test train() function
        System.out.println("=== Testing train() function ===");
        System.out.println("\nTest Case 1.1 - Basic Training");
        forest.train(data.getDataset(), 5, 3);
        System.out.println("Test Result: PASS (Training completed)");
        
        System.out.println("\nTest Case 1.2 - Training with Different Parameters");
        forest.train(data.getDataset(), 10, 5);
        System.out.println("Test Result: PASS (Training with new parameters completed)");
        
        // Test predict() function
        System.out.println("\n=== Testing predict() function ===");
        UserProfile testProfile = new UserProfile("TEST001", 70);
        testProfile.addHealthIndicator("HighBloodPressure", true);
        testProfile.addHealthIndicator("ChestPain", true);
        
        System.out.println("\nTest Case 2.1 - Basic Prediction");
        int prediction = forest.predict(testProfile);
        System.out.println("Prediction: " + (prediction == 1 ? "At Risk" : "Not At Risk"));
        System.out.println("Test Result: " + (prediction == 0 || prediction == 1 ? "PASS" : "FAIL"));
        
        System.out.println("\nTest Case 2.2 - Prediction Consistency");
        int prediction2 = forest.predict(testProfile);
        System.out.println("Second Prediction: " + (prediction2 == 1 ? "At Risk" : "Not At Risk"));
        System.out.println("Test Result: " + (prediction == prediction2 ? "PASS" : "FAIL"));
        
        // Test predictProba() function
        System.out.println("\n=== Testing predictProba() function ===");
        System.out.println("\nTest Case 3.1 - Probability Range");
        float proba = forest.predictProba(testProfile);
        System.out.println("Probability: " + proba);
        System.out.println("Test Result: " + (proba >= 0.0f && proba <= 1.0f ? "PASS" : "FAIL"));
        
        System.out.println("\nTest Case 3.2 - Probability Consistency");
        float proba2 = forest.predictProba(testProfile);
        System.out.println("Second Probability: " + proba2);
        System.out.println("Test Result: " + (Math.abs(proba - proba2) < 0.0001 ? "PASS" : "FAIL"));
        
        // Test multiple profiles
        System.out.println("\n=== Testing Multiple Profiles ===");
        UserProfile[] testProfiles = new UserProfile[3];
        
        // High risk profile
        testProfiles[0] = new UserProfile("TEST002", 80);
        testProfiles[0].addHealthIndicator("HighBloodPressure", true);
        testProfiles[0].addHealthIndicator("ChestPain", true);
        testProfiles[0].addHealthIndicator("Dizziness", true);
        
        // Medium risk profile
        testProfiles[1] = new UserProfile("TEST003", 65);
        testProfiles[1].addHealthIndicator("HighBloodPressure", true);
        testProfiles[1].addHealthIndicator("ChestPain", false);
        
        // Low risk profile
        testProfiles[2] = new UserProfile("TEST004", 45);
        testProfiles[2].addHealthIndicator("HighBloodPressure", false);
        testProfiles[2].addHealthIndicator("ChestPain", false);
        
        System.out.println("\nTest Case 4.1 - Multiple Profile Predictions");
        for (int i = 0; i < testProfiles.length; i++) {
            int pred = forest.predict(testProfiles[i]);
            float prob = forest.predictProba(testProfiles[i]);
            System.out.println("\nProfile " + (i + 1) + ":");
            System.out.println("Age: " + testProfiles[i].age);
            System.out.println("Prediction: " + (pred == 1 ? "At Risk" : "Not At Risk"));
            System.out.println("Probability: " + prob);
        }
        System.out.println("Test Result: PASS (Multiple profiles tested)");
        
        // Test edge cases
        System.out.println("\n=== Testing Edge Cases ===");
        
        System.out.println("\nTest Case 5.1 - Empty Profile");
        UserProfile emptyProfile = new UserProfile("TEST005", 0);
        int emptyPred = forest.predict(emptyProfile);
        float emptyProba = forest.predictProba(emptyProfile);
        System.out.println("Empty Profile Prediction: " + (emptyPred == 1 ? "At Risk" : "Not At Risk"));
        System.out.println("Empty Profile Probability: " + emptyProba);
        System.out.println("Test Result: PASS (Empty profile handled)");
        
        System.out.println("\nTest Case 5.2 - Maximum Age Profile");
        UserProfile maxAgeProfile = new UserProfile("TEST006", 100);
        maxAgeProfile.addHealthIndicator("HighBloodPressure", true);
        int maxAgePred = forest.predict(maxAgeProfile);
        float maxAgeProba = forest.predictProba(maxAgeProfile);
        System.out.println("Max Age Prediction: " + (maxAgePred == 1 ? "At Risk" : "Not At Risk"));
        System.out.println("Max Age Probability: " + maxAgeProba);
        System.out.println("Test Result: PASS (Maximum age handled)");
        
        // Test performance
        System.out.println("\n=== Testing Performance ===");
        System.out.println("\nTest Case 6.1 - Prediction Speed");
        long startTime = System.currentTimeMillis();
        for (int i = 0; i < 100; i++) {
            forest.predict(testProfile);
        }
        long endTime = System.currentTimeMillis();
        double avgTime = (endTime - startTime) / 100.0;
        System.out.println("Average prediction time: " + avgTime + "ms");
        System.out.println("Test Result: " + (avgTime <= 5000 ? "PASS" : "FAIL"));
        
        System.out.println("\nTest Case 6.2 - Probability Calculation Speed");
        startTime = System.currentTimeMillis();
        for (int i = 0; i < 100; i++) {
            forest.predictProba(testProfile);
        }
        endTime = System.currentTimeMillis();
        avgTime = (endTime - startTime) / 100.0;
        System.out.println("Average probability calculation time: " + avgTime + "ms");
        System.out.println("Test Result: " + (avgTime <= 5000 ? "PASS" : "FAIL"));
        
        System.out.println("\n=== All Random Forest Function Tests Completed ===\n");
        
        // Start the GUI
        StrokeRiskGUI gui = new StrokeRiskGUI();
        gui.setVisible(true);
    }
} 