# Stroke Risk Prediction System

A Java-based system that predicts stroke risk using Random Forest algorithm. The system analyzes health indicators to provide accurate risk assessments and allows comparison with similar patient profiles.

## Features

- **Risk Prediction**: Uses Random Forest algorithm for accurate predictions
- **Real-time Assessment**: Instant risk evaluation based on patient data
- **GUI Interface**: User-friendly interface for data input and visualization
- **Profile Comparison**: Compare with similar patient profiles
- **Database Management**: Store and manage patient records

## Prerequisites

- Java 11 or higher
- Git (for cloning the repository)

## Installation

1. Install Java 11 or higher from [Adoptium](https://adoptium.net/)
2. Clone the repository:
   ```bash
   git clone https://github.com/Radinb2022/StrokeRiskSystem
   cd StrokeRiskSystem
   ```

## Running the Application

### Windows
Double-click `run.bat` or run in PowerShell:
```powershell
.\run.bat
```

### Linux/Mac
```bash
javac -d . src/*.java
java src.Main
```

## Health Indicators

The system analyzes various health indicators including:
- Age
- High Blood Pressure
- Chest Pain
- Dizziness
- Fatigue & Weakness
- Swelling (Edema)
- Pain in Neck/Jaw/Shoulder/Back
- Excessive Sweating
- Chest Discomfort
- Snoring/Sleep Apnea
- Anxiety/Feeling of Doom

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## Project Structure

```
StrokeRiskSystem/
├── src/
│   ├── Main.java                 # Application entry point
│   ├── StrokeRiskGUI.java        # GUI implementation
│   ├── StrokeRiskPredictor.java  # Risk prediction logic
│   ├── RandomForest.java         # Random Forest implementation
│   ├── TreeNode.java            # Tree node structure
│   ├── UserProfile.java         # Patient data model
│   ├── KaggleData.java          # Data loading and management
│   └── Comparison.java          # Profile comparison logic
├── data/
│   └── data.csv                 # Sample dataset
├── run.bat                      # Windows batch file for easy running
└── README.md                    # This file
```

## Testing

The project includes comprehensive test cases:
- Unit tests for risk calculation
- System tests for prediction functionality
- Performance tests
- Accuracy tests

## License

This project is licensed under the MIT License.
