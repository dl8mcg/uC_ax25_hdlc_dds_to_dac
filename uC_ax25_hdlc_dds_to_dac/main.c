/*
 * main.c
 *
 * Created: 03.02.2026
 * Modified: 12.02.2026
 * Author: DL8MCG
 */ 

#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "timer.h"
#include "board.h"
#include "hdlc_irq.h"
#include "hdlc.h"

volatile uint8_t packetcnt;

#define dlyval 200000
uint32_t verz;

void tunix()
{
	while(verz < dlyval)
	{
		verz++;
		wdt_reset();
	}
	verz = 0;
}

int main(void)
{
	wdt_enable(WDTO_2S);				// Watchdog auf 2 s setzen
	TX_DDR_OUT;
	FSK_DDR_OUT;
	DAC_DDR_OUT;
	
	init_DDS();
	init_timer_ctc();					// Timer-IRQ 76800 Hz
	
	InitHDLC();
	
	while(1)
	{
		RX_ON;							// nur LED-Test
		SetAX25Text(
					0x01,               // type AX.25 V2
					"EU", 0,			// Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"Dies ist eine technische Testaussendung in AX.25 im Modus AFSK mit 1200 Baud.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		RX_OFF;							// nur LED-Test
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"EU", 0,			// Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"Diese Übertragung dient der Demonstration klassischer Packet-Radio-Technik, wie sie seit den frühen 1980er-Jahren im Amateurfunk eingesetzt wird.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"QST", 0,           // Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"Die Grundlagen dieses Verfahrens entstanden ab 1981, als die Organisation TAPR - Tucson Amateur Packet Radio - begann,"
					"erste experimentelle Packet-Radio-Systeme für Funkamateure zu entwickeln.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"ALL", 0,           // Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"TAPR veröffentlichte 1982 das erste TNC-Projekt, den TNC-1, und legte damit den Grundstein für die weltweite Verbreitung "
					"digitaler Funkdatenübertragung im Amateurfunk.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"WORLD", 0,         // Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"Kurz darauf folgten die ersten regulären Packet-Radio-Übertragungen in Nordamerika und Kanada, bevor sich das Verfahren "
					"rasch international verbreitete.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"WORLD", 0,         // Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"AX.25, das heute verwendete Protokoll, wurde aus dem X.25-Standard abgeleitet und speziell für den Amateurfunk angepasst.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"CQ", 0,			// Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"Es nutzt HDLC-Rahmen, NRZ-I-Kodierung, Bit-Stuffing und eine 16-Bit-CRC-Prüfsumme. "
					"Für APRS oder Telemetrie werden UI-Frames verwendet.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"CQ", 0,			 // Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"Die Modulation erfolgt nach dem Bell-202-Standard mit Tonfrequenzen von 1200 Hz für Mark und 2200 Hz für Space.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();

		SetAX25Text(
					0x01,               // type AX.25 V2
					"CQ", 0,			 // Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"Die Symbolrate beträgt 1200 Baud, weshalb diese Betriebsart als 1k2 AFSK bezeichnet wird. "
					"Sie ist robust, einfach zu implementieren und kompatibel mit analogen FM-Transceivern.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"CQ", 0,			// Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"Diese Aussendung dient ausschließlich technischen Zwecken, insbesondere zur Überprüfung von Modulation, Frequenzstabilität, Audiopegeln und AX.25-Dekodierung.\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
		
		SetAX25Text(
					0x01,               // type AX.25 V2
					"CQ", 0,			// Zielrufzeichen + SSID
					"nocall", 0,        // Quellrufzeichen + SSID
					0x03,               // Control (UI-Frame)
					0xF0,               // PID (No Layer 3)
					"AVR source code https://github.com/dl8mcg/uC_ax25_hdlc_dds_to_dac\n\r"
					"# %d\n\r", packetcnt++
					);
		tunix();
	}
}