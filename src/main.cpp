#include "RandomForestUI.h"
#include "Tester.h"

int main() {
    RandomForestUI ui;
    ui.run();
    Tester::runTests();
    return 0;
}