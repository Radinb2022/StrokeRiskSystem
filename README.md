# Stroke Risk Prediction System

## Overview
This is a C++ commercial-quality software system for predicting stroke risk using a custom random forest classifier. It allows users to input patient data, receive a risk prediction, add new data, retrain the model, and compare profiles. The system is menu-driven and runs in the terminal.

## How to Build and Run

1. Open a terminal and navigate to the project root directory:
   ```sh
   cd /Users/alimustafa/StrokeRiskSystem
   ```
2. Build the project:
   ```sh
   make clean
   make
   ```
3. Run the program:
   ```sh
   ./StrokeRiskSystem
   ```

## Glossary (Health Indicators)
- **High Blood Pressure**: Yes/No — Consistently high blood pressure, a major risk factor for stroke.
- **Chest Pain**: Yes/No — Discomfort or pain in the chest area, can indicate cardiovascular issues.
- **Dizziness**: Yes/No — Sensation of lightheadedness or unsteadiness, may indicate blood flow problems.
- **Fatigue & Weakness**: Yes/No — Persistent tiredness or lack of energy, may signal underlying health issues.
- **Swelling (Edema)**: Yes/No — Abnormal fluid retention in tissues, can be a sign of heart or kidney issues.
- **Pain in Neck/Jaw/Shoulder/Back**: Yes/No — Unexplained pain in these areas, sometimes indicates cardiovascular problems.
- **Excessive Sweating**: Yes/No — May be a sign of cardiovascular stress.
- **Chest Discomfort (Activity)**: Yes/No — Pain or discomfort in the chest during physical exertion.
- **Snoring/Sleep Apnea**: Yes/No — Loud snoring or interrupted breathing during sleep, linked to higher stroke risk.
- **Anxiety/Feeling of Doom**: Yes/No — Persistent anxiety or sense of impending doom, can be associated with cardiovascular events.
- **Age**: Integer — Patient's age in years.

## Example Usage
- Select option 1 to predict stroke risk for a new patient.
- Enter a unique Patient ID (the system will prompt if it already exists).
- Answer all health questions (Yes/No) and provide age.
- The system will display the risk prediction and probability.
- To add new data, select option 2 and follow the prompts.
- To compare with a similar profile of opposite risk, select option 3.
- For help, select option 4.

## Features
- Custom random forest classifier for stroke risk prediction
- Menu-driven terminal UI
- Add new patient data and retrain the model
- Compare user profile with similar opposite-risk profile
- Help and glossary available in the UI

## Project Structure
- `src/` — Source code files
- `include/` — Header files
- `data/` — Patient dataset (CSV)
- `Makefile` — Build instructions
- `README.md` — Documentation

## Dependencies
- Standard C++17 or later
- No external libraries required

## Cleaning the Build
```sh
make clean
```

## Contributing
Pull requests and suggestions are welcome. Please ensure code is well-documented and tested.
