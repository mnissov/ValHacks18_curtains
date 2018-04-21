/*
    BYJ48 Stepper motor code
    Connect :
    IN1 >> GPIO15
    IN2 >> GPIO13
    IN3 >> GPIO12
    IN4 >> GPIO14
    VCC ... 5V Prefer to use external 5V Source
    Gnd
    written By :Mohannad Rawashdeh
    https://www.instructables.com/member/Mohannad+Rawashdeh/
    28/9/2013
*/

#define IN1  15
#define IN2  13
#define IN3  12
#define IN4  14
#define stepsPrRev 4096

int Steps = 0;
boolean Direction = true;// gre
unsigned long last_time;
unsigned long currentMillis ;
int steps_left = stepsPrRev;
long time2;
void setup()
{
    Serial.begin(115200);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    // delay(1000);

}
void loop()
{
    while (steps_left > 0) {
        currentMillis = micros();
        if (currentMillis - last_time >= 1000) {
            stepper(3);
            time2 = time2 + micros() - last_time;
            last_time = micros();
            steps_left--;
        }
    }
    Serial.println(time2);
    Serial.println("Wait...!");
    delay(2000);
    Direction = !Direction;
    steps_left = stepsPrRev;
}

void stepper(int xw) {
    for (int x = 0; x < xw; x++) {
        switch (Steps) {
            case 0:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                //delay(25);
                break;
            case 1:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, HIGH);
                //delay(25);
                break;
            case 2:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                //delay(25);
                break;
            case 3:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, HIGH);
                digitalWrite(IN4, LOW);
                //delay(25);
                break;
            case 4:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                //delay(25);
                break;
            case 5:
                digitalWrite(IN1, HIGH);
                digitalWrite(IN2, HIGH);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                //delay(25);
                break;
            case 6:
                digitalWrite(IN1, HIGH);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                //delay(25);
                break;
            case 7:
                digitalWrite(IN1, HIGH);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, HIGH);
                //delay(25);
                break;
            default:
                digitalWrite(IN1, LOW);
                digitalWrite(IN2, LOW);
                digitalWrite(IN3, LOW);
                digitalWrite(IN4, LOW);
                //delay(25);
                break;
        }
        SetDirection();
    }
}
void SetDirection() {
    if (Direction == 1) {
        Steps++;
    }
    if (Direction == 0) {
        Steps--;
    }
    if (Steps > 7) {
        Steps = 0;
    }
    if (Steps < 0) {
        Steps = 7;
    }
}
