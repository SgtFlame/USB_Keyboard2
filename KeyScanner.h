#ifndef _USB_SCANNER_KEY_SCANNER_H_
#define _USB_SCANNER_KEY_SCANNER_H_

class KeyScanner
{
  public:
    KeyScanner(const uint8_t* const pRows, uint8_t rows, 
               const uint8_t* const pCols, uint8_t cols, const char* const pKeys);
    void begin(uint8_t mcp_addr);
    bool scanKeys();
    
  private:
    void setColsLow(void);
    void setColsHigh(void);
    void keyPressed(uint8_t row, uint8_t col, unsigned long scan_time);
    void checkUnpressed(unsigned long scan_time);
    
    const uint8_t m_rows;
    const uint8_t m_cols;
    Adafruit_MCP23017 m_mcp;
    const uint8_t* const m_pRows;
    const uint8_t* const m_pCols;
    const char* const m_pKeyValues;
    unsigned long* m_pDownTime;
};

#endif

