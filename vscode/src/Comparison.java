package src;

import java.util.List;

public class Comparison {
    public UserProfile findOppositeRiskProfile(UserProfile currentUser, List<UserProfile> data, StrokeRiskPredictor predictor) {
        String currentRisk = predictor.generateAlert(predictor.calculateRisk(currentUser));
        for (UserProfile u : data) {
            String otherRisk = predictor.generateAlert(predictor.calculateRisk(u));
            if (!otherRisk.equals(currentRisk)) {
                return u;
            }
        }
        return new UserProfile("None", 0);
    }
} 