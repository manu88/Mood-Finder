#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialogDelegate
{
    friend class SettingsDialog;
protected:
    SettingsDialogDelegate()
    {}

    virtual~SettingsDialogDelegate()
    {}

    virtual const QString getProjectDirectory() const = 0;
    virtual const QString getXMLName() const = 0;
    virtual const QString getLocalXMLLocation() const = 0;
    virtual const QString getServerXMLLocation() const = 0;
    virtual int           getNumBackup() const = 0;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog( SettingsDialogDelegate *delegate);
    ~SettingsDialog();

    QString getRootDir() const;
    QString getXmlName() const;
    QString getLocalXmlLocationDir() const;
    QString getServerXmlLocationDir() const;
    int     getNumBackup() const;

private slots:
    void setRootDirClicked();
    void setLocalXMLLocationClicked();
    void setServerXMLLocationClicked();
private:
    Ui::SettingsDialog *ui;

    SettingsDialogDelegate *_delegate;
};

#endif // SETTINGSDIALOG_H
