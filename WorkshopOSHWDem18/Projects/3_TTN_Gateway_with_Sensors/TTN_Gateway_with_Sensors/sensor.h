// sensor.h; 1-channel LoRa Gateway for ESP8266
// Copyright (c) 2016, 2017, 2018 Maarten Westenberg version for ESP8266
// Version 5.3.2
// Date: 2018-07-07
//
// 	based on work done by Thomas Telkamp for Raspberry PI 1ch gateway
//	and many other contributors.
//
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the MIT License
// which accompanies this distribution, and is available at
// https://opensource.org/licenses/mit-license.php
//
// NO WARRANTY OF ANY KIND IS PROVIDED
//
// Author: Maarten Westenberg (mw12554@hotmail.com)
//
// This file contains a number of compile-time settings and declarations that are
// specific to the LoRa rfm95, sx1276, sx1272 radio of the gateway.
//
//
// ------------------------------------------------------------------------------------

#if _TRUSTED_NODES >= 1
struct nodex  {
	uint32_t id;				// This i sthe decide ID (coded in 4 bytes uint32_t
	char nm[32];
};

// Add all your named and trusted nodes to this list
nodex nodes[] = {
	{ 0x260116BD , "lora-34 PIR node" },						// F=0
	{ 0x26011152 , "lora-35 temp+humi node" },					// F=0
	{ 0x2601148C , "lora-36 test node"  },						// F=0
	{ 0x26011B90 , "lora-39 temp DS18B20" },					// F=1
	{ 0x260119A6 , "lora-40 airquality" },						// F=0
	{ 0x2601117D , "lora-41 temp+humi SR04T" },
	{ 0x26011514 , "lora-43 ch1, no sensors" },					// F=1
	{ 0x26011D77 , "lora-45 not sensor" },
	{ 0x2601160F , "lora-46 HTU21 metal case" },				// F=0
	{ 0x26011E71 , "lora-47 Dallas temperature" },				// F=0
	{ 0x26011E52 , "lora-53 gas sensor" },						// F=ALL
	{ 0x02020441 , "lora-65 Waterproof temp humi" },			// F=0
	{ 0x26011b96 , "lora-50 Internal T-Beam gway" },			// F=0
	{ 0x26011999 , "lora-901 distance sensor" }					// F=0
};


#endif //_TRUSTED_NODES

#if _LOCALSERVER==1

struct codex  {
	uint32_t id;				// This is the device ID (coded in 4 bytes uint32_t
	char nm[32];				// A name string which is free to choose
	uint8_t nwkKey[16];			// The Network Session Key
	uint8_t appKey[16];			// The Application Session Key
};

// Sometimes we want to decode the sensor completely as we do in the TTN server
// This means that for all nodes we want to view the dara of, we need to provide
// he AppsSKey and the NwkSKey

// Definition of all nodes that we want to decode locally on the gateway.
//
codex decodes[] = {
	{ 0x2601148C , "lora-36", 	// F=0
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x02, 0x02, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } 
	},
	{ 0x26011b96 , "lora-50",	// F=0
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x02, 0x02, 0x04, 0x32, 0x00, 0x00, 0x00, 0x00, 0x54, 0x68, 0x69, 0x6E, 0x67, 0x73, 0x34, 0x55 }
	},
	{ 0x26011999 , "lora-901",	// F=0
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x02, 0x02, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
	}					
};
#endif //_LOCALSERVER