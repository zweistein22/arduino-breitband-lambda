#include <arduino.h>
#include "BreitBandLambda.h"


const char *pstr_lambdaErrors(int i) {
	switch (i) {
	case SerialReadChksumError: return "SerialRdChksmErr";
	case NoLambdaConnected: return "NoLambdaConnect";
	case HeatingOffButNeeded: return "HeatOffButNeeded";
	case HeatingUp: return "HeatingUp";
	case VSupplyLow: return "VSupplyLow";
	case VSupplyHigh: return "VSupplyHigh";
	case ProbeTempHigh: return "ProbeTempHigh";
	case CJ_COMM_ERROR: return "CJ_COMM_ERROR";
	case CJ_ERROR: return "CJ_ERROR";
	}
	return "Unknown";
}

int GetLambda(char *response, int len) {

	char *p = response;
	unsigned short data[4];
	int ilen = 0;

	for (int i = 0; i < 4; i++) {
		if (*p == '\0') break;
		char *start = p;
		while (*p != ';' && *p != '\0') {
			p++;

		}
		char *end = p;
		data[i] = (unsigned short)atoi(start);
		p++;

	}

	unsigned short chksum = ~(data[0] + data[1] + data[2]);
	if (chksum != data[3]) {
#ifdef DEBUGSERIAL
		DEBUGSERIAL.print("chksum=");
		DEBUGSERIAL.print(chksum);
		DEBUGSERIAL.print("  data[3]=");
		DEBUGSERIAL.print(data[3]);
#endif
		return SerialReadChksumError;
	}

	if (data[2] == 127) return NoLambdaConnected;
	if (data[1] & 1) return CJ_ERROR;
	if (data[1] & 4) return VSupplyLow;
	if (data[1] & 8) return VSupplyHigh;
	if (data[1] & 16) return CJ_COMM_ERROR;

	if (!(data[1] & 32)) {
		if (!(data[1] & 128)) {
			return HeatingOffButNeeded;
		}
		else return HeatingUp;

	}
	if (data[1] & 2) return ProbeTempHigh;

	return data[0];
}
