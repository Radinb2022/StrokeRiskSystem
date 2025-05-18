package src;

import java.util.*;
import java.time.Duration;
import java.time.Instant;

public class RandomForestUI {
    private static void printHelp() {
        System.out.println("\n--- Help: Input Fields ---");
        System.out.println("Age: Patient's age (integer)");
        System.out.println("HighBloodPressure: Yes/No");
        System.out.println("ChestPain: Yes/No");
        System.out.println("Dizziness: Yes/No");
        System.out.println("Fatigue & Weakness: Yes/No");
        System.out.println("Swelling (Edema): Yes/No");
        System.out.println("Pain in Neck/Jaw/Shoulder/Back: Yes/No");
        System.out.println("Excessive Sweating: Yes/No");
        System.out.println("Chest Discomfort (Activity): Yes/No");
        System.out.println("Snoring/Sleep Apnea: Yes/No");
        System.out.println("Anxiety/Feeling of Doom: Yes/No");
        System.out.println("-------------------------");
    }

    private static UserProfile inputUserProfile(List<UserProfile> dataset) {
        Scanner scanner = new Scanner(System.in);
        String id;
        Set<String> existingIds = new HashSet<>();
        for (UserProfile u : dataset) {
            existingIds.add(u.patientId);
        }

        while (true) {
            System.out.print("Create Your Patient ID: ");
            id = scanner.nextLine();
            if (existingIds.contains(id)) {
                System.out.print("Patient ID already exists. Use a different ID or type 'y' to update: ");
                String resp = scanner.nextLine();
                if (resp.equalsIgnoreCase("y")) {
                    break;
                }
            } else {
                break;
            }
        }

        UserProfile user = new UserProfile(id, 0);
        System.out.print("Enter Age: ");
        user.age = scanner.nextInt();
        scanner.nextLine(); // Consume newline

        List<Map.Entry<String, String>> questions = Arrays.asList(
            Map.entry("HighBloodPressure", "High Blood Pressure (Yes/No): "),
            Map.entry("ChestPain", "Chest Pain (Yes/No): "),
            Map.entry("Dizziness", "Dizziness (Yes/No): "),
            Map.entry("Fatigue & Weakness", "Fatigue & Weakness (Yes/No): "),
            Map.entry("Swelling (Edema)", "Swelling (Edema) (Yes/No): "),
            Map.entry("Pain in Neck/Jaw/Shoulder/Back", "Pain in Neck/Jaw/Shoulder/Back (Yes/No): "),
            Map.entry("Excessive Sweating", "Excessive Sweating (Yes/No): "),
            Map.entry("Chest Discomfort (Activity)", "Chest Discomfort (Activity) (Yes/No): "),
            Map.entry("Snoring/Sleep Apnea", "Snoring/Sleep Apnea (Yes/No): "),
            Map.entry("Anxiety/Feeling of Doom", "Anxiety/Feeling of Doom (Yes/No): ")
        );

        for (Map.Entry<String, String> q : questions) {
            while (true) {
                System.out.print(q.getValue());
                String input = scanner.nextLine().toLowerCase();
                if (input.equals("yes") || input.equals("no")) {
                    user.addHealthIndicator(q.getKey(), input.equals("yes"));
                    break;
                } else {
                    System.out.println("Please enter 'Yes' or 'No'.");
                }
            }
        }
        return user;
    }

    private static void printDatabase(List<UserProfile> dataset) {
        System.out.println("\n--- Patient Database ---");
        for (UserProfile u : dataset) {
            System.out.print("ID: " + u.patientId + ", Age: " + u.age + ", Indicators: ");
            for (Map.Entry<String, Boolean> kv : u.healthIndicators.entrySet()) {
                System.out.print(kv.getKey() + "=" + (kv.getValue() ? "Yes" : "No") + ", ");
            }
            System.out.println();
        }
        System.out.println("-----------------------");
        System.out.print("Enter a Patient ID to view details or press Enter to return: ");
        Scanner scanner = new Scanner(System.in);
        String sel = scanner.nextLine();
        if (!sel.isEmpty()) {
            Optional<UserProfile> found = dataset.stream()
                .filter(u -> u.patientId.equals(sel))
                .findFirst();
            if (found.isPresent()) {
                UserProfile user = found.get();
                System.out.println("\n--- Patient Profile ---");
                System.out.println("ID: " + user.patientId);
                System.out.println("Age: " + user.age);
                for (Map.Entry<String, Boolean> kv : user.healthIndicators.entrySet()) {
                    System.out.println(kv.getKey() + ": " + (kv.getValue() ? "Yes" : "No"));
                }
                System.out.println("----------------------");
            } else {
                System.out.println("Patient ID not found.");
            }
        }
    }

    public void run() {
        KaggleData data = new KaggleData();
        data.loadSampleData();
        StrokeRiskPredictor predictor = new StrokeRiskPredictor();
        predictor.train(data.getDataset(), 10, 5);
        Comparison cmp = new Comparison();
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("\n--- Stroke Risk Prediction System ---");
            System.out.println("1. Predict Stroke Risk (Random Forest)");
            System.out.println("2. Add New User Data");
            System.out.println("3. Compare with Opposite Risk Profile");
            System.out.println("4. Help");
            System.out.println("5. Exit");
            System.out.println("6. Access Database");
            System.out.print("Select an option: ");
            
            int choice = scanner.nextInt();
            scanner.nextLine(); // Consume newline

            if (choice == 1) {
                UserProfile user = inputUserProfile(data.getDataset());
                Instant start = Instant.now();
                int pred = predictor.predict(user);
                float proba = predictor.predictProba(user);
                Instant stop = Instant.now();
                long duration = Duration.between(start, stop).toMillis();

                System.out.println("Prediction: " + (pred == 1 ? "At Risk" : "Not At Risk"));
                System.out.printf("Probability (fraction of trees voting At Risk): %.2f\n", proba);
                if (proba == 0.0f || proba == 1.0f) {
                    System.out.println("[Warning] The model is very certain. This may indicate underfitting or imbalanced data.");
                }
                System.out.println("Prediction Time: " + duration + "ms");
            } else if (choice == 2) {
                UserProfile user = inputUserProfile(data.getDataset());
                predictor.addTrainingData(user, data);
                System.out.println("User data added and model updated.");
            } else if (choice == 3) {
                UserProfile user = inputUserProfile(data.getDataset());
                UserProfile opposite = cmp.findOppositeRiskProfile(user, data.getDataset(), predictor);
                if (!opposite.patientId.equals("None")) {
                    System.out.println("\nSimilar Opposite Risk Profile Found: " + opposite.patientId + ", Age: " + opposite.age);
                } else {
                    System.out.println("No opposite risk profile found.");
                }
            } else if (choice == 4) {
                printHelp();
            } else if (choice == 5) {
                break;
            } else if (choice == 6) {
                printDatabase(data.getDataset());
            } else {
                System.out.println("Invalid option. Try again.");
            }
        }
    }
} 