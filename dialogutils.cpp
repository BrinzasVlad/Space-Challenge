#include "dialogutils.h"

#include <QMessageBox>

namespace DialogUtils {
    void showError(const QString& message) {
        QMessageBox::critical(nullptr, "Error", message);
    }
}
