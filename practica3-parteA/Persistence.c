/*
 * Persistencia.c
 *
 *  Created on: 21/12/2015
 *      Author: a604409
 */
#include "Persistence.h"

int Persistence_read_int(void) {
	/**
	 * ALMSEC = LSB
	 * ALMDAY = MSB
	 */
	int n = 0;
	n |= rALMSEC;
	n |= rALMMIN << 8;
	n |= rALMHOUR << 16;
	n |= rALMDAY << 24;
	return n;
}

char Persistence_read_char(int n) {
	switch (n) {
	case 0:
		return rALMYEAR ;
	case 1:
		return rALMMON ;
	case 2:
		return rALMDAY ;
	case 3:
		return rALMHOUR ;
	case 4:
		return rALMMIN ;
	case 5:
		return rALMSEC ;
	default:
		return 0;
	}
}

void Persistence_save_int(int n) {
	rALMSEC = n & 0xFF;
	rALMMIN = (n >> 8) & 0xFF;
	rALMHOUR = (n >> 16) & 0xFF;
	rALMDAY = (n >> 24) & 0xFF;
}

void Persistence_save_char(int n, char c) {
	switch (n) {
	case 0:
		rALMYEAR = c;
		break;
	case 1:
		rALMMON = c;
		break;
	case 2:
		rALMDAY = c;
		break;
	case 3:
		rALMHOUR = c;
		break;
	case 4:
		rALMMIN = c;
		break;
	case 5:
		rALMSEC = c;
		break;
	default:
		break;
	}
}
