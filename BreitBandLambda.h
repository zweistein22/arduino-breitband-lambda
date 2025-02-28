#ifndef __BREITBANDLAMBDA_H
#define __BREITBANDLAMBDA_H

//#define INFOSERIAL(call) Serial.call
//#define INFOSERIAL(call)

#define STRINGIFY(A)  #A
#define XSTRINGIFY(A)  STRINGIFY(A)

enum _lambdaErrors {
  SerialReadChksumError=-1,
  NoLambdaConnected=-2,
  HeatingOffButNeeded=-3,
  HeatingUp=-4,
  VSupplyLow=-5,
  VSupplyHigh=-6,
  ProbeTempHigh=-7,
  CJ_COMM_ERROR = -8,
  CJ_ERROR=-9
  
};
const char *pstr_lambdaErrors(int i);
int GetLambda(char *response, int len);

#ifdef LAMBDASERIAL
class _lambdaProbe1 {
	
	static const int responseLen = 24;
	char response[responseLen];
public:
	bool initdone = false;
	int Lambda() {
		
		if (!initdone) {
			initdone = true;

			INFOSERIAL(print(XSTRINGIFY(LAMBDASERIAL)));
			INFOSERIAL(println(":"));

			LAMBDASERIAL.begin(115200);
			LAMBDASERIAL.setTimeout(200);
			//LAMBDASERIAL.print("9\r");
			//LAMBDASERIAL.print("3\r");
			//LAMBDASERIAL.print("W\r");
			LAMBDASERIAL.print("I\r");
			LAMBDASERIAL.flush();
			for (int k = 0; k < 12; k++) {
				for (int i = 0; i < sizeof(response); i++) response[i] = '\0';
				int iread = LAMBDASERIAL.readBytesUntil('\n', response, sizeof(response));

				INFOSERIAL(println(response));
				INFOSERIAL(flush());

			}
		}
		int res=0;
		int ntries=3;
		for(int j=0;j<ntries;j++){
			int read=0;
			do {
				for (int i = 0; i < sizeof(response); i++) response[i] = '\0';
				read = LAMBDASERIAL.readBytesUntil('\n', response, sizeof(response));
#ifdef DEBUGSERIAL
		//	DEBUGSERIAL.print("response[read]=");
		//	DEBUGSERIAL.print(response[read],HEX);
		//	DEBUGSERIAL.print("response[read-1]=");
		//	DEBUGSERIAL.print(response[read-1],HEX);
		  DEBUGSERIAL.println(response);
#endif	 
			}while(read==sizeof(response));
			res = GetLambda(response, read);
			if (res >= 0) {
#ifdef DEBUGSERIAL
					DEBUGSERIAL.print(" LAMBDA(read=");
					DEBUGSERIAL.print(read);
					DEBUGSERIAL.print("):");
					DEBUGSERIAL.println(response);
#endif
#ifdef DEBUGSERIAL
					DEBUGSERIAL.print("l=");
					DEBUGSERIAL.println(res);
#endif
				}
			else {
					if (res == HeatingOffButNeeded) {
						LAMBDASERIAL.print("H\r");
						LAMBDASERIAL.print("F\r");
						delay(50);

					}
#ifdef DEBUGSERIAL
					DEBUGSERIAL.println(pstr_lambdaErrors(res));
#endif
				}
			if (res != SerialReadChksumError) break;

			if(j==ntries)	INFOSERIAL(println("j==ntries"));

		}

		return res;
	}

};
_lambdaProbe1 LAMBDA1;
#endif

#ifdef LAMBDA2SERIAL
class _lambdaProbe2 {
	
	static const int responseLen = 24;
	char response[responseLen];
public:
	bool initdone = false;
	int Lambda() {
		if (!initdone) {
			initdone = true;

			INFOSERIAL(print(XSTRINGIFY(LAMBDA2SERIAL)));
			INFOSERIAL(println(":"));

			LAMBDA2SERIAL.begin(115200);
			LAMBDA2SERIAL.setTimeout(200);
			//LAMBDASERIAL.print("9\r");
			//LAMBDASERIAL.print("3\r");
			//LAMBDASERIAL.print("W\r");
			LAMBDA2SERIAL.print("I\r");
			LAMBDA2SERIAL.flush();
			for (int k = 0; k < 12; k++) {
				for (int i = 0; i < sizeof(response); i++) response[i] = '\0';
				int iread = LAMBDA2SERIAL.readBytesUntil('\n', response, sizeof(response));

				INFOSERIAL(println(response));
				INFOSERIAL(flush());

			}
		}
		int res = 0;
		int ntries = 3;
		for (int j = 0; j < ntries; j++) {
			int read = 0;
			do {
				for (int i = 0; i < sizeof(response); i++) response[i] = '\0';
				read = LAMBDA2SERIAL.readBytesUntil('\n', response, sizeof(response));
#ifdef DEBUGSERIAL
				//	DEBUGSERIAL.print("response[read]=");
				//	DEBUGSERIAL.print(response[read],HEX);
				//	DEBUGSERIAL.print("response[read-1]=");
				//	DEBUGSERIAL.print(response[read-1],HEX);
				DEBUGSERIAL.println(response);
#endif	 
		}while (read == sizeof(response));
		res = GetLambda(response, read);
		if (res >= 0) {
#ifdef DEBUGSERIAL
			DEBUGSERIAL.print(" LAMBDA(read=");
			DEBUGSERIAL.print(read);
			DEBUGSERIAL.print("):");
			DEBUGSERIAL.println(response);
#endif
#ifdef DEBUGSERIAL
			DEBUGSERIAL.print("l=");
			DEBUGSERIAL.println(res);
#endif
		}
		else {
			if (res == HeatingOffButNeeded) {
				LAMBDA2SERIAL.print("H\r");
				LAMBDA2SERIAL.print("F\r");
				delay(50);

			}
#ifdef DEBUGSERIAL
			DEBUGSERIAL.println(pstr_lambdaErrors(res));
#endif
		}
		if (res != SerialReadChksumError) break;

		if (j == ntries)	INFOSERIAL(println("j==ntries"));

	}

		return res;
	}

};
_lambdaProbe2 LAMBDA2;
#endif



#endif
