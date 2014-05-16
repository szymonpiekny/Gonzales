#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "User/AccountsProvider.h"
#include "dialogcreateaccount.h"
#include "User/Authorizer.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    UserInfo getUserInfo();
    bool isAuthorized()
    {
        return m_info.isAuthorized;
    }

private slots:
    void on_toolButtonCreateNewAccount_clicked();
    void on_pushButtonLogin_clicked();
    void onAccountCreated(const QString &str);

private:
    Ui::LoginDialog *ui;
    DialogCreateAccount m_dialogCreateAccount;
    AccountsProvider m_provider;
    UserInfo m_info;
};

#endif // LOGINDIALOG_H
