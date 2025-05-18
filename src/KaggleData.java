package src;

import java.io.*;
import java.util.*;

public class KaggleData {
    private List<UserProfile> dataset = new ArrayList<>();

    public void loadSampleData() {
        try (BufferedReader br = new BufferedReader(new FileReader("data/data.csv"))) {
            String line = br.readLine(); // skip header
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",");
                if (parts.length < 12) continue;
                String id = parts[0];
                int age = Integer.parseInt(parts[1]);
                UserProfile user = new UserProfile(id, age);
                String[] keys = {"HighBloodPressure", "ChestPain", "Dizziness", "Fatigue & Weakness", "Swelling (Edema)", "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating", "Chest Discomfort (Activity)", "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom"};
                for (int i = 0; i < 10; ++i) {
                    user.addHealthIndicator(keys[i], parts[i+2].equals("Yes"));
                }
                dataset.add(user);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void appendUser(UserProfile user) {
        dataset.add(user);
        saveToCSV("data/data.csv");
    }

    public void saveToCSV(String filename) {
        try (PrintWriter pw = new PrintWriter(new FileWriter(filename))) {
            pw.println("PatientId,Age,HighBloodPressure,ChestPain,Dizziness,Fatigue & Weakness,Swelling (Edema),Pain in Neck/Jaw/Shoulder/Back,Excessive Sweating,Chest Discomfort (Activity),Snoring/Sleep Apnea,Anxiety/Feeling of Doom");
            String[] keys = {"HighBloodPressure", "ChestPain", "Dizziness", "Fatigue & Weakness", "Swelling (Edema)", "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating", "Chest Discomfort (Activity)", "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom"};
            for (UserProfile user : dataset) {
                StringBuilder sb = new StringBuilder();
                sb.append(user.patientId).append(",").append(user.age);
                for (String k : keys) {
                    sb.append(",").append(user.healthIndicators.getOrDefault(k, false) ? "Yes" : "No");
                }
                pw.println(sb.toString());
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public List<UserProfile> getDataset() {
        return dataset;
    }
} 