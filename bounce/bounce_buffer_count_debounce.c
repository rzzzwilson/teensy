/*                                                                               
    Code to count the number of "bounces" on switch open or close.               
    The data is buffered in-memory and dumped to the serial port                 
    after the switch has settled.                                                
                                                                                 
    Derived from "bounce_buffer.c".                                              
*/                                                                               
                                                                                 
// store this number of results                                                  
const int MaxBuffer = 2048;                                                      
// after this number of samples, the switch has settled                          
//const long int BounceLimit = 4096*16;                                                    
const long int BounceLimit = 100000;                                                    
                                                                                 
bool LastState = not digitalRead(7);
unsigned long Count = 0;                                                         
int BounceCount = 0;                                                            
unsigned long CountBuffer[MaxBuffer];                                                 
bool BoolBuffer[MaxBuffer];                                                     
unsigned int Index = 0;     
                                                                                 
void setup()                                                                     
{                                                                                
    Serial.begin(38400);                                                         
    pinMode(7, INPUT);
}                                                                                
                                                                                 
void report(void)                                                                
{                                                                                
    for (unsigned int i=0; i < Index ; ++i)                                        
    {                                                                            
        unsigned long count = CountBuffer[i];                                         
        bool state = BoolBuffer[i];                                             
                                                                                 
        Serial.print(count);                                                     
        if (count == 1)                                                          
            Serial.print(" change going to state ");                             
        else                                                                     
            Serial.print(" changes going to state ");                            
                                                                                 
        if (state)                                                               
            Serial.println("OPEN");                                              
        else                                                                     
            Serial.println("CLOSED");                                            
    }                                                                            
                                                                                 
    Index = 0;                                                                     
}                                                                                
                                                                                 
void loop()                                                                      
{                                                                                
    bool state = digitalRead(7);  

    if (state == LastState)                                                     
    {                                                                            
        if (Count == BounceLimit)                                                
        {                                                                        
            // switch has settled, save bounce count                             
            CountBuffer[Index] = BounceCount;                                          
            BoolBuffer[Index] = state;       
            ++Index;                                                               
            BounceCount = 0;
        }                                                                        
    }                                                                            
    else                                                                         
    {                                                                            
        ++BounceCount;                                                          
        LastState = state;                                                      
        Count = 0;                                                               
    }                                                                            
                                                                                 
    ++Count;                                                                     
                                                                                 
    if (Count > 2000000)                                                         
        report();                                                                
}                                                                                
