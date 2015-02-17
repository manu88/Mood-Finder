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

    virtual QString getRootDir() const = 0;
    virtual void    setRootDir( const QString &newDir) = 0;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog( SettingsDialogDelegate *delegate);
    ~SettingsDialog();

private slots:
    void setRootDirClicked();

private:
    Ui::SettingsDialog *ui;

    SettingsDialogDelegate *_delegate;
};

#endif // SETTINGSDIALOG_H
