/*                                                                               
    Double blink LED and repeat every second.                                    
                                                                                 
    Using timers.  Still use the state machine approach.                         
                                                                                 
    The "something else" here is another state machine printing USB debug every  
    1.5 seconds.                                                                 
*/                                                                               
                                                                                 
// constants defining the cycle time(s) (all in milliseconds)                    
const int LedOn = 50;                                                           
const int LedOff = 50;                                                          
const int CycleWait = 1000 - 2*(LedOn + LedOff);                                 
                                                                                 
const int UsbWait = 1500;       

// basic timer period is 50msec (value in microseconds)
const int TimerWait = 50000;
                                                                                 
// teensy 3.1 has the LED on pin 13                                              
const int ledPin = LED_BUILTIN;                                                  
                                                                                 
// Create an IntervalTimer object                                                
IntervalTimer myTimer;                                                           
                                                                                 
// setup() gets called once at startup                                           
void setup(void) {                                                               
    pinMode(ledPin, OUTPUT);                                                     
    Serial.begin(38400);                                                         
    myTimer.begin(handle_timer, TimerWait);  // handle_timer() runs every whenever       
}                                                                                
                                                                                 
// loop() is continuously called                                                 
void loop()                                                                      
{                                                                                
}                                                                                
                                                                                 
// the timer interrupt routine                                                   
void handle_timer(void)                                                          
{                                                                                
    // get current time                                                          
    long currentMillis = (long) millis();                                        
                                                                                 
    // pass current time to LED state machine                                    
    led_state_machine(currentMillis);                                            
                                                                                 
    // and to the USB state machine                                              
    usb_state_machine(currentMillis);                                            
}                                                                                
                                                                                 
// a simple state machine to write debug to the USB port every 1.5 seconds       
void usb_state_machine(long time)                                                
{                                                                                
    static long next_time = 0;                                                   
    static int state = 0;                                                        
                                                                                 
    if (time > next_time)                                                        
    {                                                                            
        switch (state)                                                           
        {                                                                        
            case 0:                        // initialization                     
                next_time = time;                                                
                /* FALL THROUGH */                                               
            case 1:                        // write debug                        
                Serial.print(time, DEC);                                         
                Serial.println(" msec");                                         
                next_time += UsbWait;                                            
                state = 1;                                                       
                break;                                                           
        }                                                                        
    }                                                                            
}                                                                                
                                                                                 
// a simple state machine to flash the LED                                       
void led_state_machine(long time)                                                
{                                                                                
    static long next_time = 0;                                                   
    static int state = 0;                                                        
                                                                                 
    if (time > next_time)                                                        
    {                                                                            
        switch (state)                                                           
        {                                                                        
            case 0:                        // initialization                     
                next_time = time;                                                
                /* FALL THROUGH */                                               
            case 1:                        // first flash, ON                    
                digitalWrite(ledPin, HIGH);                                      
                next_time += LedOn;                                              
                state = 2;                                                       
                break;                                                           
            case 2:                        // first flash, OFF                   
                digitalWrite(ledPin, LOW);                                       
                next_time += LedOff;                                             
                state = 3;                                                       
                break;                                                           
            case 3:                        // second flash, ON                   
                digitalWrite(ledPin, HIGH);                                      
                next_time += LedOn;                                              
                state = 4;                                                       
                break;                                                           
            case 4:                        // second flash, OFF                  
                digitalWrite(ledPin, LOW);                                       
                next_time += CycleWait;                                          
                state = 1;                                                       
                break;                                                           
        }                                                                        
    }                                                                            
}                                                                                

