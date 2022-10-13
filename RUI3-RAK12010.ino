#include <Arduino.h>
#include "RAK12010_light.h"

bool ret;

bool has_rak12010 = false;

/**
 * @brief Callback after packet was received
 *
 * @param data Structure with the received data
 */
void receiveCallback(SERVICE_LORA_RECEIVE_T *data)
{
	Serial.printf("RX, fP %d, DR %d, RSSI %d, SNR %d\n", data->Port, data->RxDatarate, data->Rssi, data->Snr);
	for (int i = 0; i < data->BufferSize; i++)
	{
		Serial.printf("%02X", data->Buffer[i]);
	}
	Serial.print("\r\n");
}

/**
 * @brief Callback after TX is finished
 *
 * @param status TX status
 */
void sendCallback(int32_t status)
{
	Serial.printf("TX %d\n", status);
	digitalWrite(LED_GREEN, LOW);
}

/**
 * @brief Callback after join request cycle
 *
 * @param status Join result
 */
void joinCallback(int32_t status)
{
	// MYLOG("JOIN-CB", "Join result %d", status);
	if (status != 0)
	{
		if (!(ret = api.lorawan.join()))
		{
			Serial.printf("Join fail!\n");
			// if (found_sensors[OLED_ID].found_sensor)
			// {
			// 	rak1921_add_line("Join NW failed");
			// }
		}
	}
	else
	{
		Serial.printf("DR  %s\n", api.lorawan.dr.set(3) ? "Success" : "Fail");
		Serial.printf("ADR  %s\n", api.lorawan.adr.set(false) ? "Success" : "Fail");
		Serial.printf("Joined!\n");
		digitalWrite(LED_BLUE, LOW);

		// if (found_sensors[OLED_ID].found_sensor)
		// {
		// 	rak1921_add_line("Joined NW");
		// }
		digitalWrite(LED_BLUE, LOW);
	}
}

/** Regions as text array */
char *regions_list[] = {"EU433", "CN470", "RU864", "IN865", "EU868", "US915", "AU915", "KR920", "AS923", "AS923-2", "AS923-3", "AS923-4"};
/** Network modes as text array*/
char *nwm_list[] = {"P2P", "LoRaWAN", "FSK"};

void print_device_status(void)
{
	String value_str = "";
	int nw_mode = 0;
	int region_set = 0;
	uint8_t key_eui[16] = {0}; // efadff29c77b4829acf71e1a6e76f713

	Serial.println("Device Status:");
	value_str = api.system.hwModel.get();
	value_str.toUpperCase();
	Serial.printf("Module: %s\r\n", value_str.c_str());
	Serial.printf("Version: %s\r\n", api.system.firmwareVer.get().c_str());
	nw_mode = api.lorawan.nwm.get();
	Serial.printf("Network mode %s\r\n", nwm_list[nw_mode]);
	if (nw_mode == 1)
	{
		Serial.printf("Network %s\r\n", api.lorawan.njs.get() ? "joined" : "not joined");
		region_set = api.lorawan.band.get();
		Serial.printf("Region: %d\r\n", region_set);
		Serial.printf("Region: %s\r\n", regions_list[region_set]);
		if (api.lorawan.njm.get())
		{
			Serial.println("OTAA mode");
			api.lorawan.deui.get(key_eui, 8);
			Serial.printf("DevEUI = %02X%02X%02X%02X%02X%02X%02X%02X\r\n",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7]);
			api.lorawan.appeui.get(key_eui, 8);
			Serial.printf("AppEUI = %02X%02X%02X%02X%02X%02X%02X%02X\r\n",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7]);
			api.lorawan.appkey.get(key_eui, 16);
			Serial.printf("AppKey = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7],
						  key_eui[8], key_eui[9], key_eui[10], key_eui[11],
						  key_eui[12], key_eui[13], key_eui[14], key_eui[15]);
		}
		else
		{
			Serial.println("ABP mode");
			api.lorawan.appskey.get(key_eui, 16);
			Serial.printf("AppsKey = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7],
						  key_eui[8], key_eui[9], key_eui[10], key_eui[11],
						  key_eui[12], key_eui[13], key_eui[14], key_eui[15]);
			api.lorawan.nwkskey.get(key_eui, 16);
			Serial.printf("NwsKey = %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\r\n",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3],
						  key_eui[4], key_eui[5], key_eui[6], key_eui[7],
						  key_eui[8], key_eui[9], key_eui[10], key_eui[11],
						  key_eui[12], key_eui[13], key_eui[14], key_eui[15]);
			api.lorawan.daddr.set(key_eui, 4);
			Serial.printf("DevAddr = %02X%02X%02X%02X\r\n",
						  key_eui[0], key_eui[1], key_eui[2], key_eui[3]);
		}
	}
	else if (nw_mode == 0)
	{
		Serial.printf("Frequency = %d\r\n", api.lorawan.pfreq.get());
		Serial.printf("SF = %d\r\n", api.lorawan.psf.get());
		Serial.printf("BW = %d\r\n", api.lorawan.pbw.get());
		Serial.printf("CR = %d\r\n", api.lorawan.pcr.get());
		Serial.printf("Preamble length = %d\r\n", api.lorawan.ppl.get());
		Serial.printf("TX power = %d\r\n", api.lorawan.ptp.get());
	}
	else
	{
		Serial.printf("Frequency = %d\r\n", api.lorawan.pfreq.get());
		Serial.printf("Bitrate = %d\r\n", api.lorawan.pbr.get());
		Serial.printf("Deviaton = %d\r\n", api.lorawan.pfdev.get());
	}
}

void setup()
{
	pinMode(LED_BLUE, OUTPUT);
	pinMode(LED_GREEN, OUTPUT);

	// Setup the callbacks for joined and send finished
	api.lorawan.registerRecvCallback(receiveCallback);
	api.lorawan.registerSendCallback(sendCallback);
	api.lorawan.registerJoinCallback(joinCallback);

	digitalWrite(LED_BLUE, HIGH);
	digitalWrite(LED_GREEN, LOW);

	Serial.begin(115200);

	delay(5000); // Just wait for terminal

	Serial.println("RAKwireless LoRaWan OTAA Example");
	Serial.println("------------------------------------------------------");

	/*************************************
	 *
	 * LoRaWAN band setting:
	 *   EU433: 0
	 *   CN470: 1
	 *   RU864: 2
	 *   IN865: 3
	 *   EU868: 4
	 *   US915: 5
	 *   AU915: 6
	 *   KR920: 7
	 *   AS923: 8
	 *   AS923-2: 9
	 *   AS923-3: 10
	 *   AS923-4: 11
	 *
	 * ************************************/

	Serial.println("First setup LoRaWAN credentials with AT commands or WisToolBox");
	Serial.println("Then join LoRaWAN with AT+JOIN=1:0");

	Serial.println("\n------------------------------------------------------");
	print_device_status();
	Serial.println("\n------------------------------------------------------");

	has_rak12010 = init_rak12010();
	if (!has_rak12010)
	{
		Serial.println("Cannot find RAK12010");
	}
}

uint8_t data_packet[128];
float rak12010_lux_as_float = 0.0;
uint32_t rak12010_lux = 0L;

void loop()
{
	api.system.sleep.all(30000);

	digitalWrite(LED_GREEN, HIGH);
	if (has_rak12010)
	{
		rak12010_lux_as_float = read_rak12010();
		Serial.printf("Light was %.2f Lux\n", read_rak12010());

		rak12010_lux = (uint32_t)rak12010_lux_as_float;
	}
	// Generate data packet in Cayenne LPP format
	uint8_t data_packet_len = 0;
	data_packet[data_packet_len++] = 0x01;						   // packet id
	data_packet[data_packet_len++] = 0x65;						   // packet is luminosity
	data_packet[data_packet_len++] = (uint8_t)(rak12010_lux >> 8); //  Add the luminosity to the packet
	data_packet[data_packet_len++] = (uint8_t)(rak12010_lux);	   //  Add the luminosity to the packet

	// Check if we joined the network
	if (api.lorawan.njs.get())
	{
		if (api.lorawan.send(data_packet_len, data_packet, 2, false))
		{
			Serial.println("Packet enqueued");
		}
		else
		{
			Serial.println("Send failed");
		}
	}
	else
	{
		digitalWrite(LED_GREEN, LOW);
	}
}
