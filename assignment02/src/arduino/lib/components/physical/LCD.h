#ifndef __LCD_I2C_DISPLAY__
#define __LCD_I2C_DISPLAY__

#include "LiquidCrystal_I2C.h"

#define LCD_COLS 20 /** Number of Columns of the I2C Display */
#define LCD_ROWS 4 /** Number of Rows of the I2C Display */

/**
 * @brief LCD Component Helper Class
 * @authors Marco Marrelli, Margherita Zanchini, Sofia Caberletti
 */
class LCD {
public:
    /**
     * @brief Creates New LCD Instance
     * @param address I2C Bus Address of Display
     */
    LCD(LiquidCrystal_I2C* lcd);

    /**
     * @brief Initializes Display Hardware
     */
    void init();

    /**
     * @brief Removes all Content from Display
     */
    void clear();

    /**
     * @brief Writes Text at Specific Position on the I2C Display
     * @param text Text to Show
     * @param row=0 Row Position
     * @param col=0 Column Position
     */
    void print(const String& text, int row = 0, int col = 0);

    /**
     * @brief Retrieves the Current Text Displayed on the LCD
     */
    String getCurrentText();

    /**
     * @brief Provides Access to Hardware Interface
     * @return Reference Pointer to the I2C Display
     */
    // LiquidCrystal_I2C* getDisplay();

private:
    LiquidCrystal_I2C* _lcd;// = LiquidCrystal_I2C(0x27, 20, 4); /** Hardware Interface Instance */ // = LiquidCrystal_I2C(0x27, 20, 4)
    int _address; /** I2C Communication Address */
    String _currentText; /** Stores the current text displayed on the LCD */
};

#endif