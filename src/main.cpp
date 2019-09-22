#include <Arduino.h>
#include <VirtualWire.h>

const int led_pin = 13;
const int transmit_pin = 12;
const int receive_pin = 11;

byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message

void setup() {
  Serial.begin(9600);
  Serial.println("El dispositivo esta listo"  );
  vw_setup(2000);	 // Bits per sec
  pinMode(led_pin, OUTPUT);

  #ifdef EMISOR
  vw_set_tx_pin(transmit_pin);
  #endif

  #ifdef RECEPTOR
    vw_set_rx_pin(receive_pin);
    vw_rx_start();       // Start the receiver PLL running
  #endif

}

byte count = 1;

void send (char *message)
{
vw_send((uint8_t *)message, strlen(message));
vw_wait_tx(); // Wait until the whole message is gone
}

void loop() {
  #ifdef EMISOR
    char msg[8] = {'s','a','q','u','e','l','o'};
    msg[7] = count;
    digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
    vw_send((uint8_t *)msg, 7);
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(led_pin, LOW);
    delay(1000);
    count = count + 1;
  #endif

  #ifdef RECEPTOR
  if (vw_get_message(message, &messageLength)){ 
    Serial.print("Received: ");
    for (int i = 0; i < messageLength; i++)
    {
      Serial.write(message[i]);
    }
    Serial.println();
  }
  #endif
}