/*
 * hdlc_irq.h
 *
 * Created: 12.05.2024
 * Modified: 12.02.2026
 * Author: DL8MCG
 */ 

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <util/atomic.h>
#include "hdlc_irq.h"
#include "board.h"
#include "timer.h"

static const uint8_t *tx_data_ptr = 0;			// Pointer auf Datenfeld
uint8_t HDLCbyte;								// Aktuelles Byte im Schieberegister

// states
void HDLC_Start();
void HDLC_SendFlags();
void HDLC_SendData();
void HDLC_Stop();
// Funktionszeiger für state machine
void (* volatile smFSK)(void) = HDLC_Stop;      

static uint8_t flagbyte;
static uint16_t flagcnt;
uint8_t bitcnt;
uint16_t bytecnt;
bool isHDLC();

void InitHDLC()  // Init
{
    bitcnt = 0;
    cli();
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    { smFSK = HDLC_Stop; }      
}

void SendHDLC(const uint8_t * buf, uint16_t size)	// Start der Übertragung
{
    tx_data_ptr = buf;								// Pointer auf Start setzen
    
    cli();
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    { smFSK = HDLC_Start; }
        
    bitcnt = 0;
    flagbyte = 0x7E; 
    flagcnt = 30;									// Anzahl der Preamble Flags
    bytecnt = size;

    sei();
    
    while(isHDLC());				// Warten bis fertig (main loop wird während dieser Zeit blockiert)
}

void HDLC_Start()
{
    TX_ON;							// Sender ein
    flagbyte = 0x7E;				// Initial laden
    smFSK = HDLC_SendFlags;
}

void HDLC_SendFlags()				// Sendet kontinuierlich Flags 0x7E (01111110)
{
    // 1. NRZI Encoding (MSB prüfen)
    if (!(flagbyte & 0x80))			// Wenn das MSB 0 ist, toggeln. 0x80 = 10000000b
    {
        FSK_TOGGLE;					// Port-Pin toggeln, z.B, für TCM3105, als Alternative
		toggle_freq();				// schalte DDS-Frequenz um
    }
    
    // 2. Shiften für nächstes Bit
    flagbyte <<= 1;
    bitcnt = (bitcnt + 1) & 7;		// Modulo 8

    // 3. Byte zu Ende?
    if (bitcnt == 0)
    {
        flagcnt--;
        if (flagcnt == 0) 
        {
            smFSK = HDLC_SendData;			// wechseln ins Senden des Datenfeldes
        }
        else 
        {
            flagbyte = 0x7E;				// Flag neu laden, da durch Shift zerstört
        }
    }
}

void HDLC_SendData()						// Datenfeld senden
{
    // 1. Neues Byte laden, falls Bit-Counter auf 0
    if (bitcnt == 0)
    {
        if (bytecnt == 0)
        {
            
            smFSK = HDLC_Stop;		// Keine Daten mehr -> Stop
            return;
        }
        
        HDLCbyte = *tx_data_ptr++;	// Pointer post-increment: effizienter als Array-Index
        bytecnt--;
    }

    // 2. NRZI Encoding
    if (!(HDLCbyte & 0x80))		    // Prüfen auf 0-Bit (MSB)
    {
        FSK_TOGGLE;					// Port-Pin toggeln, z.B, für TCM3105, als Alternative
		toggle_freq();				// schalte DDS-Frequenz um
    }

    // 3. Shiften & Counter
    HDLCbyte <<= 1;
    bitcnt = (bitcnt + 1) & 7;
}

void HDLC_Stop()
{
	TX_OFF;					// Sender aus
}

bool isHDLC()				// wird immer noch übertragen?
{
	wdt_reset();
	return (smFSK != HDLC_Stop);
}