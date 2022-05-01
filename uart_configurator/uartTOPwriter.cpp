#include "uartTOPwriter.h"

uartTOPwriter::uartTOPwriter(
        bool isVerilog,
        std::string sDbit,
        std::string sSbTick,
        std::string sDvsr,
        std::string sDvsrBit,
        std::string sFifoW,
        std::string sFilePath)
{
    m_isVerilog = isVerilog;
    m_DBIT = sDbit;
    m_SB_TICK =  sSbTick;
    m_DVSR = sDvsr;
    m_DVSR_BIT = sDvsrBit;
    m_FIFO_W = sFifoW;

    if(isVerilog)
    {
        EXPORT_FILE_PATH = sFilePath + "/" + VERILOG_FILE_NAME;
    }
    else
    {
        EXPORT_FILE_PATH = sFilePath + "/" + VHDL_FILE_NAME;
    }
}

void uartTOPwriter::getModuleDeclaration()
{
    if(m_isVerilog)
    {
        m_uartTOPContext << "module uart" << '\n';
        m_uartTOPContext << "    #(" << '\n';
        m_uartTOPContext << "    parameter DBIT = " << m_DBIT <<", // data bits" << '\n';
        m_uartTOPContext << "              SB_TICK = "<<m_SB_TICK<<", //ticks for stop bit" << '\n';
        m_uartTOPContext << "              " << '\n';
        m_uartTOPContext << "              DVSR = "<<m_DVSR <<", //baud rate divisor" << '\n';
        m_uartTOPContext << "              " << '\n';
        m_uartTOPContext << "              DVSR_BIT = "<<m_DVSR_BIT<<", //bits for DVSR" << '\n';
        m_uartTOPContext << "              FIFO_W = "<<m_FIFO_W<<"    // addr bits for FIFO" << '\n';
        m_uartTOPContext << "    )" << '\n';
    }
}

void uartTOPwriter::getPortLits()
{
    if(m_isVerilog)
    {
        m_uartTOPContext << "    ( " << '\n';
        m_uartTOPContext << "        input wire clk, reset," << '\n';
        m_uartTOPContext << "        input wire rd_uart, wr_uart, rx," << '\n';
        m_uartTOPContext << "        input wire [7:0] w_data," << '\n';
        m_uartTOPContext << "        output wire tx_full, rx_empty, tx," << '\n';
        m_uartTOPContext << "        output wire [7:0] r_data" << '\n';
        m_uartTOPContext << "    );" << '\n';
    }
}

void uartTOPwriter::getSignalsList()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTOPContext << "    wire tick, rx_done_tick, tx_done_tick;" << '\n';
        m_uartTOPContext << "    wire tx_empty, tx_fifo_not_empty;" << '\n';
        m_uartTOPContext << "    wire [7:0] tx_fifo_out, rx_data_out;" << '\n';
    }
}


void uartTOPwriter::getInstantiations()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTOPContext << "    mod_m_counter #(.M(DVSR), .N(DVSR_BIT)) baud_gen_unit" << '\n';
        m_uartTOPContext << "        (.clk(clk), .reset(reset), .q(), .max_tick(tick));" << '\n';
        addEnter();
        m_uartTOPContext << "    uart_rx #(.DBIT(DBIT), .SB_TICK(SB_TICK)) uart_rx_unit" << '\n';
        m_uartTOPContext << "        (.clk(clk), .reset(reset), .rx(rx), .s_tick(tick)," << '\n';
        m_uartTOPContext << "         .rx_done_tick(rx_done_tick), .dout(rx_data_out));" << '\n';
        addEnter();
        m_uartTOPContext << "    fifo #(.B(DBIT), .W(FIFO_W)) fifo_rx_unit" << '\n';
        m_uartTOPContext << "        (.clk(clk), .reset(reset), .rd(rd_uart)," << '\n';
        m_uartTOPContext << "         .wr(rx_done_tick), .w_data(rx_data_out)," << '\n';
        m_uartTOPContext << "         .empty(rx_empty), .full(), .r_data(r_data));" << '\n';
        addEnter();
        m_uartTOPContext << "    fifo #(.B(DBIT), .W(FIFO_W)) fifo_tx_unit" << '\n';
        m_uartTOPContext << "        (.clk(clk), .reset(reset), .rd(tx_done_tick)," << '\n';
        m_uartTOPContext << "         .wr(wr_uart), .w_data(w_data)," << '\n';
        m_uartTOPContext << "         .empty(tx_empty), .full(tx_full), .r_data(tx_fifo_out));" << '\n';
        addEnter();
        m_uartTOPContext << "    uart_tx #(.DBIT(DBIT), .SB_TICK(SB_TICK)) uart_tx_unit" << '\n';
        m_uartTOPContext << "        (.clk(clk), .reset(reset), .tx_start(tx_fifo_not_empty)," << '\n';
        m_uartTOPContext << "         .s_tick(tick), .din(tx_fifo_out)," << '\n';
        m_uartTOPContext << "         .tx_done_tick(tx_done_tick), .tx(tx));" << '\n';

    }
}

void uartTOPwriter::getAssignments()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTOPContext << "    assign tx_fifo_not_empty = ~tx_empty;" << '\n';
    }
}


void uartTOPwriter::getEndFile()
{
    addEnter();
    if(m_isVerilog)
    {
        m_uartTOPContext << "endmodule" << '\n';
    }
}

void uartTOPwriter::addEnter()
{
    m_uartTOPContext << '\n' << '\n' ;
}

void uartTOPwriter::writeToFile()
{
    getModuleDeclaration();
    getPortLits();
    getSignalsList();
    getInstantiations();
    getAssignments();
    getEndFile();


    std::ofstream myfile;
    myfile.open (EXPORT_FILE_PATH);
    if (myfile.is_open())
    {
        myfile << m_uartTOPContext.str();
        myfile.close();
    }

}







