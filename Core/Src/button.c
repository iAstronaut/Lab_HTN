/*
 * button.c
 * Improved version with better structure and error handling
 */

#include "button.h"

// Constants
#define BUTTON_COUNT          16
#define LONG_PRESSED_TIME     2000    // 2s
#define BUTTON_READ_TIMEOUT   10      // ms
#define BUTTON_ACTIVE_LOW     0       // Button is active when input is low
#define BUTTON_INACTIVE       1

// Button mapping for physical layout
typedef enum {
    BTN_ROW1_START = 0,
    BTN_ROW1_END = 3,
    BTN_ROW2_START = 4,
    BTN_ROW2_END = 7,
    BTN_ROW3_START = 8,
    BTN_ROW3_END = 11,
    BTN_ROW4_START = 12,
    BTN_ROW4_END = 15
} ButtonRowIndex;

// Button state structure
typedef struct {
    uint16_t press_duration[BUTTON_COUNT];
    uint16_t raw_input;
    bool initialized;
} ButtonState;

static ButtonState button_state = {0};

// Private function prototypes
static uint8_t map_physical_to_logical_index(uint8_t physical_index);
static bool is_valid_button_id(uint8_t id);
static void process_button_states(void);
static HAL_StatusTypeDef read_button_input(void);

/**
 * @brief  Initialize button hardware and state
 * @retval HAL_StatusTypeDef indicating success/failure
 */
HAL_StatusTypeDef button_init(void) {
    // Set initial pin state
    HAL_StatusTypeDef status = HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, GPIO_PIN_SET);
    if (status != HAL_OK) {
        return status;
    }
    
    // Clear all button states
    memset(&button_state, 0, sizeof(ButtonState));
    button_state.initialized = true;
    
    return HAL_OK;
}

/**
 * @brief  Maps physical button index to logical button index based on layout
 * @param  physical_index: Raw index from input scanning
 * @retval Mapped logical button index
 */
static uint8_t map_physical_to_logical_index(uint8_t physical_index) {
    if (physical_index >= BTN_ROW1_START && physical_index <= BTN_ROW1_END) {
        return physical_index + 4;
    } else if (physical_index >= BTN_ROW2_START && physical_index <= BTN_ROW2_END) {
        return 7 - physical_index;
    } else if (physical_index >= BTN_ROW3_START && physical_index <= BTN_ROW3_END) {
        return physical_index + 4;
    } else if (physical_index >= BTN_ROW4_START && physical_index <= BTN_ROW4_END) {
        return 23 - physical_index;
    }
    return 0; // Should never reach here if input is valid
}

/**
 * @brief  Reads button inputs via SPI
 * @retval HAL status
 */
static HAL_StatusTypeDef read_button_input(void) {
    // Load the button states
    HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, GPIO_PIN_SET);
    
    // Read via SPI
    return HAL_SPI_Receive(&hspi1, (uint8_t*)&button_state.raw_input, 2, BUTTON_READ_TIMEOUT);
}

/**
 * @brief  Process all button states and update durations
 */
static void process_button_states(void) {
    uint16_t mask = 0x8000;
    
    for (int i = 0; i < BUTTON_COUNT; i++) {
        uint8_t logical_index = map_physical_to_logical_index(i);
        
        // Update press duration based on button state
        if (button_state.raw_input & mask) {
            button_state.press_duration[logical_index] = 0;
        } else {
            if (button_state.press_duration[logical_index] < UINT16_MAX) {
                button_state.press_duration[logical_index]++;
            }
        }
        mask >>= 1;
    }
}

/**
 * @brief  Scan matrix button states
 * @note   Should be called every READ_BUTTON_TIME ms
 * @retval HAL_StatusTypeDef indicating success/failure
 */
HAL_StatusTypeDef button_scan(void) {
    if (!button_state.initialized) {
        return HAL_ERROR;
    }
    
    HAL_StatusTypeDef status = read_button_input();
    if (status != HAL_OK) {
        return status;
    }
    
    process_button_states();
    return HAL_OK;
}

/**
 * @brief  Check if button ID is valid
 * @param  id: Button ID to check
 * @retval true if valid, false otherwise
 */
static bool is_valid_button_id(uint8_t id) {
    return (id < BUTTON_COUNT) && button_state.initialized;
}

/**
 * @brief  Check if a button is currently pressed
 * @param  id: Button ID to check [0...15]
 * @retval true if button is pressed, false if invalid or not pressed
 */
bool is_button_pressed(uint8_t id) {
    if (!is_valid_button_id(id)) {
        return false;
    }
    return button_state.press_duration[id] > 0;
}

/**
 * @brief  Check if a button is long pressed
 * @param  id: Button ID to check [0...15]
 * @retval true if button is long pressed, false if invalid or not long pressed
 */
bool is_button_long_pressed(uint8_t id) {
    if (!is_valid_button_id(id)) {
        return false;
    }
    return button_state.press_duration[id] >= (LONG_PRESSED_TIME / READ_BUTTON_TIME);
}

/**
 * @brief  Get button press duration
 * @param  id: Button ID [0...15]
 * @retval Duration in scanning cycles, 0 if invalid
 */
uint16_t get_button_press_duration(uint8_t id) {
    if (!is_valid_button_id(id)) {
        return 0;
    }
    return button_state.press_duration[id];
}