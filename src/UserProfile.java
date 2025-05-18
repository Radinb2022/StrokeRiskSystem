package src;

import java.util.HashMap;
import java.util.Map;

public class UserProfile {
    public String patientId;
    public int age;
    public Map<String, Boolean> healthIndicators;

    public UserProfile(String patientId, int age) {
        this.patientId = patientId;
        this.age = age;
        this.healthIndicators = new HashMap<>();
    }

    public void addHealthIndicator(String key, boolean value) {
        this.healthIndicators.put(key, value);
    }
} 