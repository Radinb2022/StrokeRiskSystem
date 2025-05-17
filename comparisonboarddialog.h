#ifndef COMPARISONBOARDDIALOG_H
#define COMPARISONBOARDDIALOG_H

#include <QDialog>
#include "src/UserProfile.h"    // For displaying user profile
#include "src/KaggleData.h"     // For accessing the full dataset
#include "src/Comparison.h"     // For the comparison logic

QT_BEGIN_NAMESPACE
namespace Ui { class ComparisonBoardDialog; }
QT_END_NAMESPACE

class ComparisonBoardDialog : public QDialog {
    Q_OBJECT

public:
    explicit ComparisonBoardDialog(const UserProfile& userProfile, KaggleData& allData, Comparison& comparisonLogic, QWidget *parent = nullptr);
    ~ComparisonBoardDialog();

private slots:
    void on_closeButton_clicked();

private:
    Ui::ComparisonBoardDialog *ui;
    const UserProfile& m_userProfile; // Reference to the user's profile
    KaggleData& m_allData; // Reference to all loaded data
    Comparison& m_comparisonLogic; // Reference to the comparison logic

    void populateTables(const UserProfile& yourProfile, const UserProfile& similarProfile,
                        const std::string& yourRisk, const std::string& similarRisk);
};

#endif // COMPARISONBOARDDIALOG_H
