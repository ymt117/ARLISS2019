/*
 * Speaker
 */
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

/*
 * LED
 */
#define ON 1
#define OFF 0

static const uint8_t led1 = PB1;
static const uint8_t led2 = PB15;

/*
 * CdS
 */
static const uint8_t cds = PA0;
