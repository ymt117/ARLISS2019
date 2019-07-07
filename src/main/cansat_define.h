// Speaker
#define mC 261.626
#define mD 293.665
#define mE 329.628
#define mF 349.228
#define mG 391.995
#define mA 440.000
#define mB 493.883
#define nn 0.0

#define BEAT_TIME 200

#define BOOT_UP   1
#define SHUT_DOWN 2
#define CLANNAD   3

static const uint8_t sp = PB12; // Pin connected to the Speaker

// LED
static const uint8_t led1 = PB13; // Pin connected to the LED1
static const uint8_t led2 = PB14; // Pin connected to the LED2

// CdS
static const uint8_t cds = PA0; // Pin connected to the CdS

// Switch
static const uint8_t sw = PC13; // Pin connected to the Switch
volatile uint16_t press_counter = 0;

// Motor
static const uint8_t ain1 = PC7; // Pin connected to the AIN1
static const uint8_t ain2 = PC8; // Pin connected to the AIN2
static const uint8_t pwma = PB5; // Pin connected to the PWMA

static const uint8_t bin1 = PB4; // Pin connected to the BIN1
static const uint8_t bin2 = PB10; // Pin connected to the BIN2
static const uint8_t pwmb = PA8; // Pin connected to the PWMB
