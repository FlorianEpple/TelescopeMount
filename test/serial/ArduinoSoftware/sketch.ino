void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop()
{
    // put your main code here, to run repeatedly:
    static String inputString = ""; // String to hold incoming data
    char incomingByte;

    while (Serial.available() > 0)
    {
        incomingByte = Serial.read();

        // If the incoming character is not a newline, add it to the string
        if (incomingByte != '\n')
        {
            inputString += incomingByte;
        }
        else
        {
            // If a newline is received, process the complete string
            Serial.print("Received: ");
            Serial.println(inputString);

            // Clear the string for the next input
            inputString = "";
        }
    }
}
