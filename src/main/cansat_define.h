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

static const uint8_t sp = PB12;

// LED
static const uint8_t led1 = PB13;
static const uint8_t led2 = PB14;

// CdS
static const uint8_t cds = PA0;

// Switch
static const uint8_t sw = PC13;
volatile uint16_t sw_counter = 0;
