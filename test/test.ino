#define IN1  15 // blue
#define IN2  13 // pink
#define IN3  12 // yellow
#define IN4  14 // orange
#define timeDelay 750;
#define FULL_ROTATION 4076

const int phases1[] = {0, 0, 0, 0, 0, 1, 1, 1};
const int phases2[] = {0, 0, 0, 1, 1, 1, 0, 0};
const int phases3[] = {0, 1, 1, 1, 0, 0, 0, 0};
const int phases4[] = {1, 1, 0, 0, 0, 0, 0, 1};
int Phase = 0;

void setup()
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop()
{
    stepper(FULL_ROTATION);
    stepper(-FULL_ROTATION);
}

void stepper(int count)
{
    int rotationDirection = count < 1 ? -1 : 1;
    count *= rotationDirection;
    for (int x = 0; x < count; x++)
    {
        digitalWrite(IN1, phases1[Phase]);
        digitalWrite(IN2, phases2[Phase]);
        digitalWrite(IN3, phases3[Phase]);
        digitalWrite(IN4, phases4[Phase]);
        IncrementPhase(rotationDirection);
        delayMicroseconds(timeDelay);
    }
}

void IncrementPhase(int rotationDirection)
{
    Phase += 8;
    Phase += rotationDirection;
    Phase %= 8;
}
