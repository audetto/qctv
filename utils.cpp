#include "utils.h"

#include "hk_error.h"

#include <QMessageBox>

void showError(QWidget *parent, const HK_Error & error)
{
    const QString msg = QString("Last error = %1\n%2").arg(QString::number(error.getError()), QString::fromUtf8(error.getMessage()));
    QMessageBox::critical(parent, QString::fromUtf8(error.what()), msg);
}
