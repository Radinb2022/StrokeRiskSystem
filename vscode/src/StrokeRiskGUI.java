package src;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.List;
import java.util.stream.Collectors;

public class StrokeRiskGUI extends JFrame {
    private static final String DATASET_PATH = "data/stroke_risk_dataset 3.csv";
    private StrokeRiskML mlModel;
    private JTextField ageField;
    private Map<String, JCheckBox> healthIndicators;
    private JTextArea resultArea;
    private JTextArea databaseArea;
    private JPanel comparisonPanel;
    private List<UserProfile> similarProfiles;
    private JTextField searchField;
    private JPanel searchResultsPanel;

    public StrokeRiskGUI() {
        mlModel = new StrokeRiskML();
        mlModel.loadAndPreprocessData(DATASET_PATH);
        mlModel.trainModel();
        similarProfiles = new ArrayList<>();

        setTitle("Stroke Risk Prediction System - Using Dataset 3");
        setSize(1200, 800);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // Create main panel with tabs
        JTabbedPane tabbedPane = new JTabbedPane();
        tabbedPane.addTab("Risk Assessment", createInputPanel());
        tabbedPane.addTab("Similar Profiles", createComparisonPanel());
        tabbedPane.addTab("Database", createDatabasePanel());
        tabbedPane.addTab("Search Profiles", createSearchPanel());
        add(tabbedPane);
    }

    private JPanel createInputPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        // Input section
        JPanel inputPanel = new JPanel(new GridBagLayout());
        inputPanel.setBorder(BorderFactory.createTitledBorder("Patient Information"));
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 5, 5, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        // Age input with validation
        gbc.gridx = 0;
        gbc.gridy = 0;
        inputPanel.add(new JLabel("Age (numeric):"), gbc);
        
        gbc.gridx = 1;
        ageField = new JTextField(10);
        ageField.setToolTipText("Enter age (0-120)");
        inputPanel.add(ageField, gbc);

        // Health indicators
        healthIndicators = new HashMap<>();
        String[] indicators = {
            "Chest Pain", "Shortness of Breath", "Irregular Heartbeat", 
            "Fatigue & Weakness", "Dizziness", "Swelling (Edema)",
            "Pain in Neck/Jaw/Shoulder/Back", "Excessive Sweating",
            "Persistent Cough", "Nausea/Vomiting", "High Blood Pressure",
            "Chest Discomfort (Activity)", "Cold Hands/Feet",
            "Snoring/Sleep Apnea", "Anxiety/Feeling of Doom"
        };

        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.gridwidth = 2;
        JLabel binaryLabel = new JLabel("Health Indicators (Yes/No):");
        binaryLabel.setToolTipText("Select 'Yes' for present symptoms, 'No' for absent");
        inputPanel.add(binaryLabel, gbc);

        for (int i = 0; i < indicators.length; i++) {
            gbc.gridy = i + 2;
            JCheckBox cb = new JCheckBox(indicators[i]);
            cb.setToolTipText("Check if symptom is present");
            healthIndicators.put(indicators[i], cb);
            inputPanel.add(cb, gbc);
        }

        // Predict button
        gbc.gridy = indicators.length + 2;
        JButton predictButton = new JButton("Predict Risk");
        predictButton.addActionListener(e -> predictRisk());
        inputPanel.add(predictButton, gbc);

        panel.add(inputPanel, BorderLayout.WEST);

        // Results section
        JPanel resultsPanel = new JPanel(new BorderLayout());
        resultsPanel.setBorder(BorderFactory.createTitledBorder("Prediction Results"));
        
        resultArea = new JTextArea(10, 30);
        resultArea.setEditable(false);
        resultArea.setLineWrap(true);
        resultArea.setWrapStyleWord(true);
        resultArea.setFont(new Font("Arial", Font.PLAIN, 14));
        JScrollPane scrollPane = new JScrollPane(resultArea);
        resultsPanel.add(scrollPane, BorderLayout.CENTER);

        panel.add(resultsPanel, BorderLayout.CENTER);

        return panel;
    }

    private JPanel createComparisonPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        comparisonPanel = new JPanel(new GridLayout(0, 1, 5, 5));
        comparisonPanel.setBorder(BorderFactory.createTitledBorder("Similar Profiles"));
        JScrollPane scrollPane = new JScrollPane(comparisonPanel);
        panel.add(scrollPane, BorderLayout.CENTER);

        return panel;
    }

    private JPanel createDatabasePanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        
        databaseArea = new JTextArea();
        databaseArea.setEditable(false);
        databaseArea.setFont(new Font("Monospaced", Font.PLAIN, 14));
        JScrollPane scrollPane = new JScrollPane(databaseArea);
        panel.add(scrollPane, BorderLayout.CENTER);
        
        return panel;
    }

    private JPanel createSearchPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // Search controls
        JPanel searchControls = new JPanel(new FlowLayout(FlowLayout.LEFT));
        searchField = new JTextField(10);
        searchField.setToolTipText("Enter patient ID (e.g., P1, P2, etc.)");
        
        JButton searchButton = new JButton("Search");
        searchButton.addActionListener(e -> searchProfiles());
        
        // Add dataset info label
        JLabel datasetLabel = new JLabel("Searching in: " + DATASET_PATH);
        datasetLabel.setForeground(new Color(100, 100, 100));
        
        // Add help label
        JLabel helpLabel = new JLabel("Enter patient ID (e.g., P1, P2)");
        helpLabel.setForeground(new Color(100, 100, 100));
        
        searchControls.add(new JLabel("Patient ID:"));
        searchControls.add(searchField);
        searchControls.add(searchButton);
        searchControls.add(Box.createHorizontalStrut(20));
        searchControls.add(datasetLabel);
        searchControls.add(Box.createHorizontalStrut(20));
        searchControls.add(helpLabel);

        // Search results
        searchResultsPanel = new JPanel(new GridLayout(0, 1, 5, 5));
        searchResultsPanel.setBorder(BorderFactory.createTitledBorder("Search Results"));
        JScrollPane scrollPane = new JScrollPane(searchResultsPanel);

        panel.add(searchControls, BorderLayout.NORTH);
        panel.add(scrollPane, BorderLayout.CENTER);

        return panel;
    }

    private void predictRisk() {
        try {
            int age = Integer.parseInt(ageField.getText().trim());
            if (age < 0 || age > 120) {
                JOptionPane.showMessageDialog(this, 
                    "Please enter a valid age between 0 and 120.", 
                    "Invalid Age", 
                    JOptionPane.ERROR_MESSAGE);
                return;
            }

            UserProfile user = new UserProfile("temp_" + System.currentTimeMillis(), age);

            for (Map.Entry<String, JCheckBox> entry : healthIndicators.entrySet()) {
                user.addHealthIndicator(entry.getKey(), entry.getValue().isSelected());
            }

            String prediction = mlModel.getDetailedPrediction(user);
            resultArea.setText(prediction);
            updateDatabaseView(user, prediction);
            findSimilarProfiles(user);
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, 
                "Please enter a valid numeric age.", 
                "Input Error", 
                JOptionPane.ERROR_MESSAGE);
        }
    }

    private void searchProfiles() {
        String inputId = searchField.getText().trim();
        if (inputId.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Please enter a Patient ID", "Search Error", JOptionPane.WARNING_MESSAGE);
            return;
        }

        // Convert search input to proper format (e.g., "1" -> "P1")
        final String searchId = !inputId.toUpperCase().startsWith("P") ? "P" + inputId : inputId;

        searchResultsPanel.removeAll();
        List<UserProfile> matches = mlModel.getTrainingData().stream()
            .filter(profile -> profile.patientId.equalsIgnoreCase(searchId))
            .collect(Collectors.toList());

        if (matches.isEmpty()) {
            JLabel noResults = new JLabel("No matching profiles found in " + DATASET_PATH);
            noResults.setHorizontalAlignment(SwingConstants.CENTER);
            searchResultsPanel.add(noResults);
        } else {
            for (UserProfile profile : matches) {
                JPanel profilePanel = createDetailedProfilePanel(profile);
                searchResultsPanel.add(profilePanel);
            }
        }

        searchResultsPanel.revalidate();
        searchResultsPanel.repaint();
    }

    private JPanel createDetailedProfilePanel(UserProfile profile) {
        JPanel panel = new JPanel(new BorderLayout(5, 5));
        panel.setBorder(BorderFactory.createEtchedBorder());

        // Profile info
        JPanel infoPanel = new JPanel(new GridLayout(0, 1));
        JLabel idLabel = new JLabel("Patient ID: " + profile.patientId);
        idLabel.setFont(new Font("Monospaced", Font.BOLD, 14));
        infoPanel.add(idLabel);
        infoPanel.add(new JLabel("Age: " + profile.age));
        
        // Health indicators
        JPanel indicatorsPanel = new JPanel(new GridLayout(0, 1));
        indicatorsPanel.add(new JLabel("Health Indicators:"));
        for (Map.Entry<String, Boolean> entry : profile.healthIndicators.entrySet()) {
            if (!entry.getKey().equals("stroke")) {
                JLabel indicatorLabel = new JLabel("• " + entry.getKey() + ": " + 
                    (entry.getValue() ? "Yes (1)" : "No (0)"));
                if (entry.getValue()) {
                    indicatorLabel.setForeground(new Color(220, 53, 69)); // Red for positive indicators
                }
                indicatorsPanel.add(indicatorLabel);
            }
        }

        // Risk prediction
        int risk = mlModel.predictRisk(profile);
        JLabel riskLabel = new JLabel("Risk Prediction: " + (risk == 1 ? "At Risk (1)" : "Not At Risk (0)"));
        riskLabel.setForeground(risk == 1 ? new Color(220, 53, 69) : new Color(40, 167, 69));

        // Stroke outcome if available
        boolean hadStroke = profile.healthIndicators.getOrDefault("stroke", false);
        JLabel outcomeLabel = new JLabel("Stroke Outcome: " + (hadStroke ? "Yes (1)" : "No (0)"));
        outcomeLabel.setForeground(hadStroke ? new Color(220, 53, 69) : new Color(40, 167, 69));

        panel.add(infoPanel, BorderLayout.WEST);
        panel.add(indicatorsPanel, BorderLayout.CENTER);
        
        JPanel rightPanel = new JPanel(new GridLayout(2, 1));
        rightPanel.add(riskLabel);
        rightPanel.add(outcomeLabel);
        panel.add(rightPanel, BorderLayout.EAST);

        return panel;
    }

    private void findSimilarProfiles(UserProfile currentUser) {
        comparisonPanel.removeAll();
        similarProfiles.clear();

        // Find profiles with similar age (±5 years) and at least 3 matching symptoms
        for (UserProfile profile : mlModel.getTrainingData()) {
            if (Math.abs(profile.age - currentUser.age) <= 5) {
                int matchingSymptoms = 0;
                for (Map.Entry<String, Boolean> entry : currentUser.healthIndicators.entrySet()) {
                    if (entry.getValue() && profile.healthIndicators.getOrDefault(entry.getKey(), false)) {
                        matchingSymptoms++;
                    }
                }
                if (matchingSymptoms >= 3) {
                    similarProfiles.add(profile);
                }
            }
        }

        // Sort by number of matching symptoms
        similarProfiles.sort((p1, p2) -> {
            int match1 = countMatchingSymptoms(currentUser, p1);
            int match2 = countMatchingSymptoms(currentUser, p2);
            return Integer.compare(match2, match1);
        });

        // Count how many similar profiles are at risk
        int atRiskCount = 0;
        for (UserProfile profile : similarProfiles) {
            if (profile.healthIndicators.getOrDefault("stroke", false)) {
                atRiskCount++;
            }
        }

        // If majority of similar profiles are at risk, update current user's prediction
        if (atRiskCount > similarProfiles.size() / 2) {
            currentUser.addHealthIndicator("stroke", true);
        }

        // Display top 5 similar profiles
        for (int i = 0; i < Math.min(5, similarProfiles.size()); i++) {
            UserProfile profile = similarProfiles.get(i);
            JPanel profilePanel = createDetailedProfilePanel(profile);
            comparisonPanel.add(profilePanel);
        }

        // Add a note about similar profiles if found
        if (!similarProfiles.isEmpty()) {
            JLabel noteLabel = new JLabel(String.format(
                "Found %d similar profiles (%d at risk). Prediction adjusted based on similar cases.",
                similarProfiles.size(), atRiskCount));
            noteLabel.setForeground(new Color(100, 100, 100));
            comparisonPanel.add(noteLabel);
        }

        comparisonPanel.revalidate();
        comparisonPanel.repaint();
    }

    private int countMatchingSymptoms(UserProfile user1, UserProfile user2) {
        int matches = 0;
        for (Map.Entry<String, Boolean> entry : user1.healthIndicators.entrySet()) {
            if (entry.getValue() && user2.healthIndicators.getOrDefault(entry.getKey(), false)) {
                matches++;
            }
        }
        return matches;
    }

    private Color getRiskColor(double risk) {
        if (risk >= 0.8) return new Color(220, 53, 69); // Red
        if (risk >= 0.6) return new Color(255, 193, 7);  // Yellow
        if (risk >= 0.4) return new Color(255, 152, 0);  // Orange
        return new Color(40, 167, 69);  // Green
    }

    private void updateDatabaseView(UserProfile user, String prediction) {
        StringBuilder sb = new StringBuilder();
        sb.append("Patient ID: ").append(user.patientId).append("\n");
        sb.append("Age: ").append(user.age).append("\n");
        sb.append("Health Indicators:\n");
        
        for (Map.Entry<String, Boolean> entry : user.healthIndicators.entrySet()) {
            sb.append("- ").append(entry.getKey()).append(": ")
              .append(entry.getValue() ? "Yes" : "No").append("\n");
        }
        
        sb.append("\nPrediction:\n").append(prediction);
        sb.append("\n----------------------------------------\n");
        
        databaseArea.append(sb.toString());
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            StrokeRiskGUI gui = new StrokeRiskGUI();
            gui.setVisible(true);
        });
    }
} 