/*
 * my_display.c
 * Improved version with better structure and flexibility
 */

#include "my_display.h"

// Constants for display settings
#define MAX_DIGITS 2
#define FONT_SIZE 24
#define LABEL_OFFSET 24

typedef struct {
    uint32_t text_color;
    uint32_t background_color;
} DisplayColors;

typedef struct {
    uint8_t x;
    uint8_t y;
    const char* label;
} DisplayPosition;

typedef struct {
    DisplayColors colors;
    struct {
        DisplayPosition second;
        DisplayPosition minute;
        DisplayPosition hour;
        DisplayPosition day;
        DisplayPosition date;
        DisplayPosition month;
        DisplayPosition year;
    } positions;
} DisplayConfig;

// Global display configuration
static DisplayConfig display_config;

// Initialize default display configuration
void init_display_config(void) {
    // Set colors
    display_config.colors.text_color = GREEN;
    display_config.colors.background_color = BLACK;
    
    // Configure time positions and labels
    display_config.positions.second = (DisplayPosition){150, 100, "sec"};
    display_config.positions.minute = (DisplayPosition){110, 100, "min"};
    display_config.positions.hour = (DisplayPosition){70, 100, "hour"};
    
    // Configure date positions and labels
    display_config.positions.day = (DisplayPosition){20, 154, "Day"};
    display_config.positions.date = (DisplayPosition){70, 154, "DD"};
    display_config.positions.month = (DisplayPosition){110, 154, "MM"};
    display_config.positions.year = (DisplayPosition){150, 154, "YY"};
}

// Generic display function for numbers
static void display_number(const DisplayPosition* pos, uint8_t number, bool show) {
    uint32_t color = show ? display_config.colors.text_color : 
                           display_config.colors.background_color;
    
    lcd_ShowIntNum(pos->x, pos->y, number, MAX_DIGITS, 
                   color, display_config.colors.background_color, FONT_SIZE);
}

// Special display function for day with "CN" handling
static void display_day_number(uint8_t number, bool show) {
    if (!show) {
        display_number(&display_config.positions.day, number, false);
        return;
    }

    if (number == 1) {
        lcd_ShowStr(display_config.positions.day.x, 
                   display_config.positions.day.y,
                   "CN",
                   display_config.colors.text_color,
                   display_config.colors.background_color,
                   FONT_SIZE, 0);
    } else {
        display_number(&display_config.positions.day, number, true);
    }
}

// Public display functions using the generic display function
void dis_sec(uint8_t number, bool show) {
    display_number(&display_config.positions.second, number, show);
}

void dis_min(uint8_t number, bool show) {
    display_number(&display_config.positions.minute, number, show);
}

void dis_hour(uint8_t number, bool show) {
    display_number(&display_config.positions.hour, number, show);
}

void dis_day(uint8_t number, bool show) {
    display_day_number(number, show);
}

void dis_date(uint8_t number, bool show) {
    display_number(&display_config.positions.date, number, show);
}

void dis_month(uint8_t number, bool show) {
    display_number(&display_config.positions.month, number, show);
}

void dis_year(uint8_t number, bool show) {
    display_number(&display_config.positions.year, number, show);
}

// Display all labels
void display_text(void) {
    lcd_Clear(BLACK);
    
    // Helper macro to display label
    #define DISPLAY_LABEL(position) \
        lcd_ShowStr(position.x, position.y - LABEL_OFFSET, \
                   position.label, \
                   display_config.colors.text_color, \
                   display_config.colors.background_color, \
                   FONT_SIZE, 0)
    
    // Display all labels
    DISPLAY_LABEL(display_config.positions.day);
    DISPLAY_LABEL(display_config.positions.second);
    DISPLAY_LABEL(display_config.positions.minute);
    DISPLAY_LABEL(display_config.positions.hour);
    DISPLAY_LABEL(display_config.positions.date);
    DISPLAY_LABEL(display_config.positions.month);
    DISPLAY_LABEL(display_config.positions.year);
    
    #undef DISPLAY_LABEL
}

// New functions to customize display
void set_display_colors(uint32_t text_color, uint32_t bg_color) {
    display_config.colors.text_color = text_color;
    display_config.colors.background_color = bg_color;
}

void set_position(DisplayPosition* pos, uint8_t x, uint8_t y) {
    pos->x = x;
    pos->y = y;
}