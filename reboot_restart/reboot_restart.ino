/*
  Reload test.

  Code to reload a Teensy 3.1/3.2 from software.
  The #define OPERATION can be set to one of three values:
      DO_NOTHING  just flash the LED
      DO_REBOOT   will reboot the Teensy, reload from the PC
      DO_RESET    will restart the program
 */

#define DO_NOTHING  0
#define DO_REBOOT   1
#define DO_RESTART  2

#define OPERATION   DO_REBOOT


// Should restart Teensy 3, will also disconnect USB during restart
#define RESTART_ADDR       0xE000ED0C
#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

// Use the builtin LED
int led = LED_BUILTIN;

/* restart the Teensy, without invoking bootloader. */
void restart(void)
{
  // 0000101111110100000000000000100
  // Assert [2]SYSRESETREQ
  WRITE_RESTART(0x5FA0004);  
}

/* reboot the Teensy, invoking bootloader. */
void reboot(void)
{
  _reboot_Teensyduino_();
}

// flash the LED quickly
void flash_led(int period)
{
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(period);             // wait a bit
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
}

// the setup routine runs once when you press reset:
void setup()
{                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);

  Serial.begin(115200);

  flash_led(500);
  delay(1000);
  Serial.print("setup()\n");

#if OPERATION == DO_REBOOT
  Serial.print("setup: will reboot\n");
#elif OPERATION == DO_RESTART
  Serial.print("setup: will restart\n");
#else
  Serial.print("setup: will flash the LED\n");
#endif
}

// the loop routine runs over and over again forever:
void loop()
{
#if OPERATION == DO_REBOOT
  Serial.println("loop(): rebooting");
  delay(1000);               // wait for a second
  reboot();
  Serial.println("After reboot, shouldn't see this!?");
#elif OPERATION == DO_RESTART
  Serial.println("loop(): restarting");
  delay(1000);               // wait for a second
  restart();
  Serial.println("After restart, shouldn't see this!?");
#else
  Serial.println("loop(): flashing the LED");
  delay(1000);               // wait for a second
  for (int i = 0; i < 3; ++i)
  {
    flash_led(75);
    delay(75);                 // wait a bit
  }
#endif
}



