#include <Arduino.h>
#include <VirtualWire.h>

const int led_pin = 13;
const int transmit_pin = 12;
const int receive_pin = 11;

byte message[VW_MAX_MESSAGE_LEN];        // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message

void setup()
{
  Serial.begin(9600);
  Serial.println("El dispositivo esta listo");
  vw_setup(2000); // Bits per sec
  pinMode(led_pin, OUTPUT);

#ifdef EMISOR || defined EMISOR2
  vw_set_tx_pin(transmit_pin);
#endif

#if defined RECEPTOR || defined RECEPTOR2
  vw_set_rx_pin(receive_pin);
  vw_rx_start(); // Start the receiver PLL running
#endif
}

byte count = 1;

void send(char *message)
{
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone
}

void loop()
{
#ifdef EMISOR
  char msg[8] = {'s', 'a', 'q', 'u', 'e', 'l', 'o'};
  msg[7] = count;
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, 7);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(1000);
  count = count + 1;
#endif

#ifdef RECEPTOR
  if (vw_get_message(message, &messageLength))
  {
    Serial.print("Received: ");
    for (int i = 0; i < messageLength; i++)
    {
      Serial.write(message[i]);
    }
    Serial.println();
  }
#endif

#ifdef EMISOR2
  String str;
  char buf[VW_MAX_MESSAGE_LEN];

  // Ejemplo de envío int
  int dataInt = millis() / 1000;
  str = "i" + String(dataInt);          /// Convertir a string
  str.toCharArray(buf, sizeof(buf));    // Convertir a char array
  vw_send((uint8_t *)buf, strlen(buf)); // Enviar array
  vw_wait_tx();                         // Esperar envio

  // Ejemplo de envío float
  float dataFloat = 3.14;
  str = "f" + String(dataFloat);        // Convertir a string
  str.toCharArray(buf, sizeof(buf));    // Convertir a char array
  vw_send((uint8_t *)buf, strlen(buf)); // Enviar array
  vw_wait_tx();                         // Esperar envio

  delay(200);
#endif

#ifdef RECEPTOR2
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  // Recibir dato
  if (vw_get_message((uint8_t *)buf, &buflen))
  {
    String dataString;
    if ((char)buf[0] == 'i')
    {
      for (int i = 1; i < buflen; i++)
      {
        dataString.concat((char)buf[i]);
      }
      int dataInt = dataString.toInt(); // Convertir a int
      Serial.print("Int: ");
      Serial.println(dataInt);
    }
    else if ((char)buf[0] == 'f')
    {
      for (int i = 1; i < buflen; i++)
      {
        dataString.concat((char)buf[i]);
      }
      float dataFloat = dataString.toFloat(); // Convertir a float
      Serial.print("Float: ");
      Serial.println(dataFloat);
    }
  }
#endif
}