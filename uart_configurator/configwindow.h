#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ConfigWindow; }
QT_END_NAMESPACE

class ConfigWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();

public slots:
    void exportUART();
    void chooseExportPath();
    void restoreValues();

private:
    void initialize();
    void calculateBaudRateDivisor();

    bool getValues();

    Ui::ConfigWindow *ui;

    std::string m_DBIT;
    std::string m_SB_TICK;
    std::string m_DVSR;
    std::string m_DVSR_BIT;
    std::string m_FIFO_W;
    std::string m_exportPath;
    bool m_isVerilog = true;
};
#endif // CONFIGWINDOW_H
