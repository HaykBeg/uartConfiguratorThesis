#include "configwindow.h"
#include "ui_configwindow.h"
#include "counterwriter.h"
#include "fifowriter.h"
#include "uartTXwriter.h"
#include "uartRXwriter.h"
#include "uartTOPwriter.h"
#include <QIntValidator>
#include <QFileDialog>
#include <stdlib.h>

ConfigWindow::ConfigWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    initialize();
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::chooseExportPath()
{
    QString folderName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"),"");
    std::string path = folderName.toStdString();
    if(!path.empty())
    {
        m_exportPath = path;
        ui->folder_lbl->setText(folderName);
    }
}

void ConfigWindow::exportUART()
{
    if(getValues())
    {
        ui->error_lbl->setVisible(false);
        counterWriter* cw = new counterWriter(
                    m_isVerilog,
                    m_exportPath);
        cw->writeToFile();

        fifoWriter * fw = new fifoWriter(
                    m_isVerilog,
                    m_exportPath);
        fw->writeToFile();

        uartRXwriter * urxw = new uartRXwriter(
                    m_isVerilog,
                    m_exportPath);
        urxw->writeToFile();

        uartTXwriter * utxw = new uartTXwriter(
                    m_isVerilog,
                    m_exportPath);
        utxw->writeToFile();

        uartTOPwriter * topw = new uartTOPwriter(
                m_isVerilog,
                m_DBIT,
                m_SB_TICK,
                m_DVSR,
                m_DVSR_BIT,
                m_FIFO_W,
                m_exportPath);
        topw->writeToFile();

        delete topw;

        ui->succ_lbl->setVisible(true);
    }
    else
    {
        ui->succ_lbl->setVisible(false);
        ui->error_lbl->setVisible(true);
    }
}

bool ConfigWindow::getValues()
{
    if( ui->dbit_ledit->text().isEmpty()
        ||   ui->sb_tick_ledit->text().isEmpty()
        ||   ui->dvsr_bit_ledit->text().isEmpty()
        ||   ui->fifo_w_ledit->text().isEmpty()
        ||   ui->b_rate_combo->currentText().isEmpty())
    {
        return false;
    }

    m_DBIT = ui->dbit_ledit->text().toStdString();
    m_SB_TICK = ui->sb_tick_ledit->text().toStdString();
    m_DVSR_BIT = ui->dvsr_bit_ledit->text().toStdString();
    m_FIFO_W = ui->fifo_w_ledit->text().toStdString();

    calculateBaudRateDivisor();

    if(ui->language_verilog->isChecked())
    {
        m_isVerilog = true;
    }
    else
    {
        m_isVerilog = false;
    }

    return true;
}

void ConfigWindow::restoreValues()
{
    ui->dbit_ledit->setText("8");
    ui->sb_tick_ledit->setText("16");
    ui->b_rate_combo->setCurrentIndex(3);
    ui->dvsr_bit_ledit->setText("8");
    ui->clk_ledit->setText("50000000");
    ui->fifo_w_ledit->setText("2");
}

void ConfigWindow::calculateBaudRateDivisor()
{
    std::string baudRate =  ui->b_rate_combo->currentText().toStdString();
    std::string clkHz =  ui->clk_ledit->text().toStdString();
    int iBaudRate = std::atoi( baudRate.c_str() );
    int iClk = std::atoi( clkHz.c_str() );
    int bitCounter = 16;
    m_DVSR = std::to_string(iClk / (bitCounter * iBaudRate));
}

void ConfigWindow::initialize()
{
    ui->language_vhdl->setEnabled(false);
    ui->error_lbl->setVisible(false);
    ui->succ_lbl->setVisible(false);

    ui->dbit_ledit->setValidator( new QIntValidator(0, 1000000000, this) );
    ui->sb_tick_ledit->setValidator( new QIntValidator(0, 1000000000, this) );
    ui->dvsr_bit_ledit->setValidator( new QIntValidator(0, 1000000000, this) );
    ui->fifo_w_ledit->setValidator( new QIntValidator(0, 1000000000, this) );

    connect(ui->export_btn, SIGNAL(pressed()), this, SLOT(exportUART()));
    connect(ui->restore_btn, SIGNAL(pressed()), this, SLOT(restoreValues()));
    connect(ui->choose_btn, SIGNAL(pressed()), this, SLOT(chooseExportPath()));

}

