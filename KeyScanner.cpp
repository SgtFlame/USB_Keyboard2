#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Adafruit_MCP23017.h>

#include "KeyScanner.h"

#define BOUNCE_DELAY 20

KeyScanner::KeyScanner(const uint8_t* const pRows, uint8_t rows, 
                       const uint8_t* const pCols, uint8_t cols, 
                       const char* const pKeyValues)
: m_pRows(pRows), m_rows(rows), m_pCols(pCols), m_cols(cols), m_pKeyValues(pKeyValues)
{
  // init m_pDownTime
  m_pDownTime = (unsigned long*)malloc(sizeof(unsigned long) * m_rows * m_cols);
  for(uint8_t x = 0; x < m_rows * m_cols; x++)
  {
    *(m_pDownTime + x) = 0;
  }
}

void
KeyScanner::begin(uint8_t mcp_addr)
{
  m_mcp.begin(mcp_addr);

  // Setup the mcp rows as input and pullup to HIGH
  for(uint8_t row = 0; row < m_rows; row++)
  {
    m_mcp.pinMode(m_pRows[row], INPUT);
    m_mcp.pullUp(m_pRows[row], HIGH);
  }
  
  // Setup the mcp cols as outputs and initialize to LOW
  for(uint8_t col = 0; col < m_cols; col++)
  {
    m_mcp.pinMode(m_pCols[col], OUTPUT);
  }
  setColsLow();
}

void
KeyScanner::setColsLow(void)
{
  uint16_t ba = m_mcp.readGPIOAB();
  ba = ba & 0xff00;
  m_mcp.writeGPIOAB(ba);
}

void
KeyScanner::setColsHigh(void)
{
  uint16_t ba = m_mcp.readGPIOAB();
  ba = ba | 0x00ff;
  m_mcp.writeGPIOAB(ba);
}

void
KeyScanner::keyPressed(uint8_t row, uint8_t col, unsigned long scan_time)
{
  // Check for newly depressed key
  if(m_pDownTime[row * m_cols + col] == 0)
  {
    //lcd.print(key_values[row][col]);
    Keyboard.press((const char)m_pKeyValues[row * m_cols + col]);
  }

  // Update the time that the key was last seen as "pressed"
  m_pDownTime[row * m_cols + col] = scan_time;
}

// check for keys that are no longer pressed
void
KeyScanner::checkUnpressed(unsigned long scan_time)
{
  if(scan_time < BOUNCE_DELAY)
  {
    return;
  }          
  unsigned long target_time = scan_time - BOUNCE_DELAY;
  for(uint8_t row = 0; row < m_rows; row++)
  {
    for(uint8_t col = 0; col < m_cols; col++)
    {
      unsigned long down_time = m_pDownTime[row * m_cols + col];
      if(down_time > 0 && down_time < target_time)
      {
        Keyboard.release((const char)m_pKeyValues[row * m_cols + col]);
        m_pDownTime[row * m_cols + col] = 0;
      }
    }
  }
}

bool
KeyScanner::scanKeys() {
  unsigned long scan_time = millis();
  int value = LOW;
  
  // Scan the rows to see if a key is pressed
  for(uint8_t row = 0; row < m_rows; row++)
  {
    if(!m_mcp.digitalRead(m_pRows[row]))
    {
      value = HIGH;
      setColsHigh();
      // Iterate through the columns to see which column
      for(uint8_t col = 0; col < m_cols; col++)
      {
        m_mcp.digitalWrite(m_pCols[col], LOW);
        if(!m_mcp.digitalRead(m_pRows[row]))
        {
          keyPressed(row, col, scan_time);
        }
        m_mcp.digitalWrite(m_pCols[col], HIGH);
      }
      setColsLow();
    }
  }
  
  checkUnpressed(scan_time);
  return (value == HIGH);
}

