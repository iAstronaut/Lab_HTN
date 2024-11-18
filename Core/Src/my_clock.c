/*
 * my_clock.c
 *
 *  Created on: Nov 23, 2023
 *      Author: HP
 */

#include "my_clock.h"

typedef enum {
    CLOCK_OK,
    CLOCK_ERROR_INVALID_TIME,
    CLOCK_ERROR_INVALID_DATE,
    CLOCK_ERROR_HARDWARE
} ClockError;

typedef enum {
    DISPLAY,
    CHANGE_TIME, 
    ALARM
} ClockState;

typedef enum {
    SECOND,
    MINUTE,
    HOUR,
    DAY,
    DATE,
    MONTH,
    YEAR
} TimeField;

typedef enum {
    RELEASE,
    PRESSED,
    LONG_PRESSED
} ButtonState;

typedef enum {
    ON,
    OFF
} BlinkState;

typedef struct {
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} TimeData;

typedef struct {
    TimeData time;
    bool enabled;
    bool triggered;
} AlarmData;

#define MAX_ALARMS 5
#define BLINKING_TIME 500
#define INCREASE_TIME 100

// clock.c
static struct {
    ClockState state;
    TimeField changing_field;
    BlinkState blink_state;
    TimeData current_time;
    AlarmData alarms[MAX_ALARMS];
    ButtonState button_states[3];
    uint32_t blink_timer;
    uint32_t increase_timer;
} clock_ctx;

static void update_display(void);
static void handle_buttons(void);
static void process_alarms(void);
static bool is_valid_time(const TimeData *time);
static void increase_field(TimeData *time, TimeField field);

void clock_init(void) {
    hw_init();
    memset(&clock_ctx, 0, sizeof(clock_ctx));
    clock_ctx.state = DISPLAY;

    // Load saved state
    clock_load_state();
}

void clock_run(void) {
    handle_buttons();
    
    switch(clock_ctx.state) {
        case DISPLAY:
            ds3231_read_time(&clock_ctx.current_time);
            update_display();
            process_alarms();
            break;
            
        case CHANGE_TIME:
            if(clock_ctx.blink_timer == 0) {
                clock_ctx.blink_state = (clock_ctx.blink_state == ON) ? OFF : ON;
                clock_ctx.blink_timer = BLINKING_TIME;
                update_display();
            }
            break;
            
        case ALARM:
            if(clock_ctx.blink_timer == 0) {
                clock_ctx.blink_state = (clock_ctx.blink_state == ON) ? OFF : ON;
                clock_ctx.blink_timer = BLINKING_TIME;
                update_display();
            }
            break;
    }
    
    // Update timers
    if(clock_ctx.blink_timer > 0) clock_ctx.blink_timer--;
    if(clock_ctx.increase_timer > 0) clock_ctx.increase_timer--;
    
    // Save state periodically
    if (clock_ctx.state != DISPLAY) {
        clock_save_state();
    }
}

static void handle_buttons(void) {
    // Mode button (Button 0)
    switch(clock_ctx.button_states[0]) {
        case RELEASE:
            if(is_button_pressed(0)) {
                switch(clock_ctx.state) {
                    case DISPLAY:
                        clock_ctx.state = CHANGE_TIME;
                        clock_ctx.changing_field = SECOND;
                        break;
                    case CHANGE_TIME:
                        clock_ctx.state = ALARM;
                        break;
                    case ALARM:
                        clock_ctx.state = DISPLAY;
                        break;
                }
                clock_ctx.button_states[0] = PRESSED;
                update_display();
            }
            break;
            
        case PRESSED:
            if(!is_button_pressed(0)) {
                clock_ctx.button_states[0] = RELEASE;
            }
            break;
    }
    
    // Increase button (Button 1)
    switch(clock_ctx.button_states[1]) {
        case RELEASE:
            if(is_button_pressed(1)) {
                if(clock_ctx.state != DISPLAY) {
                    if(clock_ctx.state == CHANGE_TIME) {
                        increase_field(&clock_ctx.current_time, 
                                    clock_ctx.changing_field);
                    } else {
                        increase_field(&clock_ctx.alarms[0].time, 
                                    clock_ctx.changing_field);
                    }
                    update_display();
                }
                clock_ctx.button_states[1] = PRESSED;
            }
            break;
            
        case PRESSED:
            if(!is_button_pressed(1)) {
                clock_ctx.button_states[1] = RELEASE;
            } else if(is_button_long_pressed(1)) {
                clock_ctx.button_states[1] = LONG_PRESSED;
                clock_ctx.increase_timer = INCREASE_TIME;
            }
            break;
            
        case LONG_PRESSED:
            if(!is_button_pressed(1)) {
                clock_ctx.button_states[1] = RELEASE;
            } else if(clock_ctx.increase_timer == 0) {
                if(clock_ctx.state == CHANGE_TIME) {
                    increase_field(&clock_ctx.current_time, 
                                clock_ctx.changing_field);
                } else {
                    increase_field(&clock_ctx.alarms[0].time, 
                                clock_ctx.changing_field);
                }
                clock_ctx.increase_timer = INCREASE_TIME;
                update_display();
            }
            break;
    }
    
    // Set button (Button 2)
    switch(clock_ctx.button_states[2]) {
        case RELEASE:
            if(is_button_pressed(2)) {
                if(clock_ctx.state == CHANGE_TIME) {
                    // Save current field
                    ds3231_write_time(&clock_ctx.current_time);
                }
                // Move to next field
                clock_ctx.changing_field = (clock_ctx.changing_field + 1) % 7;
                clock_ctx.button_states[2] = PRESSED;
                update_display();
            }
            break;
            
        case PRESSED:
            if(!is_button_pressed(2)) {
                clock_ctx.button_states[2] = RELEASE;
            }
            break;
    }
}

static void update_display(void) {
    char buf[32];
    
    // Clear display areas
    lcd_clear_area(0, 0, 240, 320);
    
    // Show mode
    switch(clock_ctx.state) {
        case DISPLAY:
            lcd_show_string(20, 30, "MODE: DISPLAY", 0xFFFF, 0x0000, 24, true);
            break;
        case CHANGE_TIME:
            lcd_show_string(20, 30, "MODE: CHANGE TIME", 0xFFFF, 0x0000, 24, true);
            break;
        case ALARM:
            lcd_show_string(20, 30, "MODE: ALARM", 0xFFFF, 0x0000, 24, true);
            break;
    }
    
    // Show time
    const TimeData *time = (clock_ctx.state == ALARM) ? 
                          &clock_ctx.alarms[0].time : 
                          &clock_ctx.current_time;
    
    // Format and show each field
    snprintf(buf, sizeof(buf), "%02d", time->hour);
    lcd_show_string(70, 80, buf, 0xFFFF, 
        (clock_ctx.changing_field == HOUR && 
         clock_ctx.blink_state == OFF) ? 0x0000 : 0x001F, 
        32, true);
    
    lcd_show_string(102, 80, ":", 0xFFFF, 0x0000, 32, true);
    
    snprintf(buf, sizeof(buf), "%02d", time->min);
    lcd_show_string(118, 80, buf, 0xFFFF,
        (clock_ctx.changing_field == MINUTE && 
         clock_ctx.blink_state == OFF) ? 0x0000 : 0x001F,
        32, true);
    
    // Only show seconds in display mode
    if(clock_ctx.state == DISPLAY) {
        lcd_show_string(150, 80, ":", 0xFFFF, 0x0000, 32, true);
        snprintf(buf, sizeof(buf), "%02d", time->sec);
        lcd_show_string(166, 80, buf, 0xFFFF, 0x0000, 32, true);
    }
    
    // Show date
    static const char *days[] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    snprintf(buf, sizeof(buf), "%s %02d/%02d/20%02d", 
             days[time->day], time->date, time->month, time->year);
    lcd_show_string(40, 130, buf, 0xFFFF,
        (clock_ctx.changing_field >= DAY && 
         clock_ctx.blink_state == OFF) ? 0x0000 : 0x001F,
        24, true);
}

static void process_alarms(void) {
    for(int i = 0; i < MAX_ALARMS; i++) {
        if(!clock_ctx.alarms[i].enabled || 
           clock_ctx.alarms[i].triggered) continue;
            
        const TimeData *alarm = &clock_ctx.alarms[i].time;
        const TimeData *now = &clock_ctx.current_time;
        
        if(alarm->year == now->year && 
           alarm->month == now->month &&
           alarm->date == now->date &&
           alarm->hour == now->hour &&
           alarm->min == now->min) {
            // Trigger alarm
            clock_ctx.alarms[i].triggered = true;
            lcd_show_string(10, 200, "WAKE UP", 0xFFFF, 0x001F, 24, true);
        }
    }
}

static bool is_valid_time(const TimeData *time) {
    if(time->hour > 23 || time->min > 59 || time->sec > 59 ||
       time->day == 0 || time->day > 7 ||
       time->month == 0 || time->month > 12 ||
       time->date == 0) return false;
       
    // Check days in month
    static const uint8_t days_in_month[] = 
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint8_t max_days = days_in_month[time->month];
    
    // Adjust for leap year
    if(time->month == 2 && time->year % 4 == 0) max_days = 29;
    
    if(time->date > max_days) return false;
    
    return true;
}

static void increase_field(TimeData *time, TimeField field) {
    switch(field) {
        case SECOND:
            time->sec = (time->sec + 1) % 60;
            break;
        case MINUTE:
            time->min = (time->min + 1) % 60;
            break;
        case HOUR:
            time->hour = (time->hour + 1) % 24;
            break;
        case DAY:
            time->day = (time->day % 7) + 1;
            break;
        case DATE:
            time->date++;
            if(!is_valid_time(time)) time->date = 1;
            break;
        case MONTH:
            time->month = (time->month % 12) + 1;
            // Adjust date if needed
            while(!is_valid_time(time)) time->date--;
            break;
        case YEAR:
            time->year = (time->year + 1) % 100;
            break;
    }
}

ClockError clock_set_time(const TimeData *time) {
    if(!is_valid_time(time)) return CLOCK_ERROR_INVALID_TIME;
    return ds3231_write_time(time);
}

ClockError clock_get_time(TimeData *time) {
    return ds3231_read_time(time);
}

ClockError clock_set_alarm(uint8_t alarm_id, const TimeData *time) {
    if(alarm_id >= MAX_ALARMS) return CLOCK_ERROR_INVALID_TIME;
    if(!is_valid_time(time)) return CLOCK_ERROR_INVALID_TIME;
    
    memcpy(&clock_ctx.alarms[alarm_id].time, time, sizeof(TimeData));
    return CLOCK_OK;
}

void clock_enable_alarm(uint8_t alarm_id, bool enable) {
    if(alarm_id < MAX_ALARMS) {
        clock_ctx.alarms[alarm_id].enabled = enable;
        clock_ctx.alarms[alarm_id].triggered = false;
    }
}

void clock_save_state(void) {
    eeprom_save_time(&clock_ctx.current_time);
    eeprom_save_alarms(clock_ctx.alarms, MAX_ALARMS);
}

void clock_load_state(void) {
    eeprom_load_time(&clock_ctx.current_time);
    eeprom_load_alarms(clock_ctx.alarms, MAX_ALARMS);
}