/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Modified by Alex TC @TCRobotics for 1 BricoLabs ESP8266 LoRa shield
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "M",
 * using frequency and encryption settings matching those of
 * the The Things Network.
 *
 * This uses ABP (Activation-by-personalisation), where a DevAddr and
 * Session keys are preconfigured (unlike OTAA, where a DevEUI and
 * application key is configured, while the DevAddr and session keys are
 * assigned/generated in the over-the-air-activation procedure).
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!
 *
 * To use this sketch, first register your application and device with
 * the things network, to set or generate a DevAddr, NwkSKey and
 * AppSKey. Each device should have their own unique values for these
 * fields.
 *
 * Do not forget to define the radio type correctly in LMIC´s config.h.
 *
 * IRQ not working (dio0 disconnected),
 * it does not detect TC complete, so it only sends one time :(
 *
 *******************************************************************************/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

#define LORA_BAND   868E6 // LoRa Band (Europe)

//Pinout! Customized for BricoLabs ESP8266 LoRa shield!
const lmic_pinmap lmic_pins = {
  .nss  = D8,
  .rxtx = LMIC_UNUSED_PIN,
  .rst  = LMIC_UNUSED_PIN,
  //.dio  = { LMIC_UNUSED_PIN, LMIC_UNUSED_PIN, LMIC_UNUSED_PIN },
  .dio  = { D1, D2, LMIC_UNUSED_PIN }, // use only with jumpers D3-D1 and D4-D2
};

static const PROGMEM u1_t NWKSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const u1_t PROGMEM APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const u4_t DEVADDR = { 0x00000000 } ;

static uint8_t mydata[] = "M";
static osjob_t sendjob;

const int channel = 0;
const int dr = DR_SF9;
const unsigned TX_INTERVAL = 5; // Schedule TX every this many seconds

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }
void do_send(osjob_t* j);

void displayInfo(String info){
  Serial.println(info);
}

void onEvent (ev_t ev) {
  switch(ev)
  {
      case EV_SCAN_TIMEOUT:   displayInfo("SCAN_TIMEOUT"); break;
      case EV_BEACON_FOUND:   displayInfo("BEACON_FOUND"); break;
      case EV_BEACON_MISSED:  displayInfo("BEACON_MISSED"); break;
      case EV_BEACON_TRACKED: displayInfo("BEACON_TRACKED"); break;
      case EV_JOINING:        displayInfo("JOINING"); break;
      case EV_RFU1:           displayInfo("RFU1"); break;
      case EV_LOST_TSYNC:     displayInfo("LOST_TSYNC"); break;
      case EV_RESET:          displayInfo("RESET"); break;
      case EV_RXCOMPLETE:     displayInfo("RXCOMPLETE"); break;
      case EV_LINK_DEAD:      displayInfo("LINK_DEAD"); break;
      case EV_LINK_ALIVE:     displayInfo("LINK_ALIVE"); break;
      case EV_JOIN_FAILED:    displayInfo("JOIN_FAILED"); break;
      case EV_REJOIN_FAILED:  displayInfo("REJOIN_FAILED"); break;
      case EV_JOINED:
        displayInfo("REJOIN_FAILED");
        LMIC_setLinkCheckMode(2);
        break;
      case EV_TXCOMPLETE:
          displayInfo("TXCOMPLETE");

          if (LMIC.txrxFlags & TXRX_ACK)
            displayInfo("Received ACK");
          if (LMIC.dataLen) {
            Serial.print("RX:");
            Serial.print(LMIC.dataLen);
            Serial.print(" bytes RSSI:");
            Serial.print(LMIC.rssi);
            Serial.print("SNR:");
            Serial.println(LMIC.snr);
          }
          // Schedule next transmission
          os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
          break;
    default:
          Serial.print("Undefined event: ");
          Serial.println(ev);
          break;
  }
}

void do_send(osjob_t* j){
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND)
  {
      displayInfo("OP_TXRXPEND, not sent");
  }
  else
  {
      // Prepare upstream data transmission at the next possible time.
      LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
      displayInfo("PACKET QUEUED");
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nBricoLabs ESP8266 LoRa shield node TTN");

pinMode(D1, INPUT);
pinMode(D2, INPUT);

  os_init(); // LMIC init

  LMIC_reset(); // Reset MAC state. Session &pending data transfers discarded.

  // Set static session parameters. Instead of dynamically establishing a session
  // by joining the network, precomputed session parameters are be provided.
  #ifdef PROGMEM
  // On AVR, these values are stored in flash and only copied to RAM
  // once. Copy them to a temporary buffer here, LMIC_setSession will
  // copy them into a buffer of its own again.
  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
  #else
  // If not running an AVR with PROGMEM, just use the arrays directly
  LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
  #endif

  // Set up the channels used by the Things Network, which corresponds
  // to the defaults of most gateways. Without this, only three base
  // channels from the LoRaWAN specification are used, which certainly
  // works, so it is good for debugging, but can overload those
  // frequencies, so be sure to configure the full frequency range of
  // your network here (unless your network autoconfigures them).
  // Setting up channels should happen after LMIC_setSession, as that
  // configures the minimal channel set.
  LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
  LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band

  LMIC_setLinkCheckMode(0); // Disable link check validation
  LMIC.dn2Dr = DR_SF9;  // TTN uses SF9 for its RX2 window.
  LMIC_setDrTxpow(DR_SF7,14); // Set data rate and transmit power for uplink

  // Disables all channels, except for the one defined above, and sets the
  // data rate (SF). This only affects uplinks; for downlinks the default
  // channels or the configuration from the OTAA Join Accept are used.
  // Not LoRaWAN compliant; FOR TESTING ONLY!
  for (int i = 0; i < 9; i++) // For EU; for US use i<71
  {
    if (i != channel) LMIC_disableChannel(i);
  }
  LMIC_setDrTxpow(dr, 14); // Set data rate (SF) and transmit power for uplink
  LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
  do_send(&sendjob); // Start job
}

void loop() {
  os_runloop_once();
}
