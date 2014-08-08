bool debug = false;
bool states[] = {true, true, true, true, true, true, true, true};

const int CVALUES = 8;
const int DELAY = 5;
const int PRESSURE_THRESHOLD = 200;

bool * mugVals[CVALUES];
int current = 0;

void setup() {
    if (debug)
        Serial.begin(115200);
        
    for(int i = 0; i < 8; i++){
        mugVals[i] = (bool*) calloc(CVALUES, sizeof(bool));
    }
}

void loop() {
    if(current < CVALUES)
        current++;

    for (int mug = 0; mug < 8; mug++)
    {
        bool newState = readBool(A0 + mug, 2);
        pushValue(mugVals[mug], newState);
        
        if(checkDoubleTap(mugVals[mug])){
            eventDoubleTap(mug);
        } else {
            bool isEqual = false;
            int i = 1;
            while(i < CVALUES){
                if(newState == mugVals[mug][i]){
                    isEqual = true;
                } else {
                    isEqual = false;
                    break;
                }
                i++;
            }
            
            if(isEqual && newState != states[mug])
            {
                states[mug] = newState;
                event(mug);
            }
        }
    }

    delay(DELAY);

}

bool checkDoubleTap(bool * vals) {
    bool firstTapFound = false;
    bool gapFound = false;
    bool secondTapFound = false;

    for (int i = 0; i < CVALUES; i++){
        if(!firstTapFound){
            firstTapFound = firstTapFound || vals[i];
        }
        else if(!gapFound){
            gapFound = gapFound || (!vals[i]);
        }
        else if(!secondTapFound){
            secondTapFound = secondTapFound || vals[i];
        }
    }
    
    if (firstTapFound && gapFound && secondTapFound){
        memset(vals, 0, CVALUES * sizeof(bool));
        current = 0; // if we delete the whole array, we should start filling it up again, right?
        return true;
    }
    return false;
}

void pushValue(bool * vals, bool newValue){
    if(current >= CVALUES){ // if array is full we push from the right and shift old values to the left
        int i = 1;
        while(i < CVALUES){
            vals[i-1] = vals[i];
            i++;
        }
        vals[CVALUES-1] = newValue;
    } else {
        vals[current] = newValue;
    }
}

bool readBool(int pin, int count)
{
    int val = readSensor(pin, count);
    return (val > PRESSURE_THRESHOLD);
}

int readSensor(int pin, int count)
{
    int total = 0;
    for (int i = 0; i < count; i++)
    {
        total += analogRead(pin);
        delay(5);
    }
    
    int val = (int)(total/count);
    
    return val;
}

void event(int mug)
{
    if (debug)
    {
        Serial.print("Mug ");
        Serial.print(mug, DEC);
        Serial.print(" is ");        
        
        if (states[mug]) //mug down
        {
            Serial.println("down!");
        }
        else //mug up
        {
           Serial.println("up!");
        }
    }
    else
    {
        char msg[63];
        if (states[mug]) //mug down
        {
            sprintf(msg, "{\"id\":%d,\"d\":\"down\"}", mug);
            Spark.publish("mug", msg, 60, PRIVATE);            
        }
        else //mug up
        {
            sprintf(msg, "{\"id\":%d,\"d\":\"up\"}", mug);
            Spark.publish("mug", msg, 60, PRIVATE);            
        }        
        
        
    }
}

void eventDoubleTap(int mug)
{
    if (debug)
    {
        Serial.print("Coaster ");
        Serial.print(mug, DEC);
        Serial.println(" is tapped");        
    }
    else
    {
        char msg[63];
        sprintf(msg, "{\"id\":%d,\"d\":\"tap\"}", mug);
        Spark.publish("mug", msg, 60, PRIVATE);            
    }
}

/*
RAW TEST DATA from sensors

A0:107|A1:111|A2:104|A3:89|A4:76|A5:108|A6:82|A7:77
A0:107|A1:116|A2:101|A3:89|A4:76|A5:107|A6:79|A7:77
A0:108|A1:111|A2:98|A3:94|A4:77|A5:99|A6:81|A7:81
A0:110|A1:112|A2:106|A3:93|A4:75|A5:105|A6:84|A7:80
A0:107|A1:112|A2:94|A3:89|A4:77|A5:106|A6:78|A7:79
A0:103|A1:113|A2:97|A3:97|A4:75|A5:102|A6:84|A7:84
A0:108|A1:115|A2:100|A3:92|A4:77|A5:106|A6:84|A7:81
A0:107|A1:113|A2:100|A3:91|A4:76|A5:110|A6:84|A7:79
A0:102|A1:110|A2:96|A3:86|A4:77|A5:107|A6:79|A7:73
A0:101|A1:111|A2:102|A3:94|A4:76|A5:108|A6:84|A7:79
A0:107|A1:110|A2:97|A3:93|A4:77|A5:105|A6:86|A7:82
A0:107|A1:111|A2:97|A3:95|A4:76|A5:111|A6:86|A7:82
A0:105|A1:111|A2:99|A3:90|A4:73|A5:101|A6:85|A7:81
A0:108|A1:108|A2:102|A3:95|A4:76|A5:97|A6:84|A7:81
A0:109|A1:110|A2:97|A3:96|A4:75|A5:102|A6:85|A7:79
A0:106|A1:112|A2:95|A3:87|A4:77|A5:100|A6:85|A7:83
A0:110|A1:118|A2:100|A3:88|A4:79|A5:101|A6:85|A7:83
A0:105|A1:111|A2:92|A3:92|A4:76|A5:102|A6:87|A7:

*/
