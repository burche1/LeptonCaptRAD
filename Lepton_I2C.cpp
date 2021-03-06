#include <stdio.h>
#include "Lepton_I2C.h"
#include "leptonSDKEmb32PUB/LEPTON_SDK.h"
#include "leptonSDKEmb32PUB/LEPTON_SYS.h"
#include "leptonSDKEmb32PUB/LEPTON_Types.h"
#include "leptonSDKEmb32PUB/LEPTON_AGC.h"
#include "leptonSDKEmb32PUB/LEPTON_OEM.h"
#include "leptonSDKEmb32PUB/LEPTON_RAD.h"
bool _connected;

LEP_CAMERA_PORT_DESC_T _port;
LEP_SYS_FPA_TEMPERATURE_KELVIN_T fpa_temp_kelvin;
LEP_RESULT result;
//****************************************************
//variables used on ROI_temp()
LEP_RAD_ROI_T spotmeterRoi;
LEP_RAD_SPOTMETER_OBJ_KELVIN_T spotmeterObj;
//****************************************************

int lepton_connect() {
	int res = (int)LEP_OpenPort(1, LEP_CCI_TWI, 400, &_port);
	_connected = true;
	return res;
}


int lepton_temperature(){
	if(!_connected)
		lepton_connect();
	result = ((LEP_GetSysFpaTemperatureKelvin(&_port, &fpa_temp_kelvin)));
	printf("FPA temp kelvin: %i, code %i\n", fpa_temp_kelvin, result);
	return (fpa_temp_kelvin/100);
}


float raw2Celsius(float raw){
	float ambientTemperature = 25.0;
	float slope = 0.0217;
	return slope*raw+ambientTemperature-177.77;
}

//****************************************************************************
float ROI_temp(int colum, int row){
	//defining structure to set ROI
	spotmeterRoi.startRow = row;
	spotmeterRoi.startCol = colum;
	spotmeterRoi.endRow = row;
	spotmeterRoi.endCol = colum;
	//Setting ROI
	LEP_SetRadSpotmeterRoi(&_port, spotmeterRoi);
	//Getting pixel RAD temp.
    LEP_GetRadSpotmeterObjInKelvinX100(&_port, &spotmeterObj);
	return spotmeterObj.radSpotmeterValue;
}
//****************************************************************************

void lepton_perform_ffc() {
	printf("performing FFC...\n");
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}

	int res = (int)LEP_RunSysFFCNormalization(&_port);
	if (res != 0) {
		printf("FFC not successful\n");
		printf("error code: %d\n", res);
	} else {
		printf("FFC successful\n");
	}

}

void lepton_restart() {
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}
	printf("restarting...\n");
	int res = (int)LEP_RunOemReboot(&_port);

	if(res != 0) {
		printf("restart unsuccessful with error: %d\n", res);
	} else {
		printf("restart successful!\n");
	}

}



void lepton_disable_agc() {
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}
	printf("Disable AGC...\n");
	
	
	int res = (int)LEP_SetAgcEnableState(&_port, LEP_AGC_DISABLE);
	if(res != 0) {
		printf("Disable AGC unsuccessful with error: %d\n", res);
	} else {
		printf("Disable AGC successful!%d:\n", res);
	}
		
}
void lepton_enable_agc() {
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}
	printf("Enable AGC...\n");
	
	
	int res = (int)LEP_SetAgcEnableState(&_port, LEP_AGC_ENABLE);

	if(res != 0) {
		printf("Enable AGC unsuccessful with error: %d\n", res);
	} else {
		printf("Enable AGC successful! %d:\n", res);
	}
}
	
	


void lepton_serial_number() {
	//verify connection
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}

	LEP_RAD_ENABLE_E rad_state;
	LEP_GetRadTLinearEnableState(&_port, &rad_state);
	LEP_GetRadEnableState(&_port, &rad_state);
	printf("RadTlinear: %d\n", rad_state);
	printf("RadEnableState: %d\n", rad_state);



	LEP_SYS_TELEMETRY_ENABLE_STATE_E tele_state;
	LEP_GetSysTelemetryEnableState(&_port, &tele_state);
	printf("TelemetryEnableState: %i\n", tele_state);
	

	//LEP_SetSysTelemetryEnableState(&_port, LEP_TELEMETRY_ENABLED);
	//printf("telemetry enable");
		
}



void lepton_sysfpatempk() {
	//verify connection
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}

//unsigned int LeptonVariation::getRadSpotmeterObjInKelvinX100()

    LEP_RAD_SPOTMETER_OBJ_KELVIN_T spotmeterObj;
    LEP_GetRadSpotmeterObjInKelvinX100(&_port, &spotmeterObj);
    printf("TEMP (GetRadSpotmeterObjInKelvinX100) : %d \n", spotmeterObj.radSpotmeterValue);

	//result = ((LEP_GetSysFpaTemperatureKelvin(&_port, &fpa_temp_kelvin)));
	
	LEP_GetSysFpaTemperatureKelvin(&_port, &fpa_temp_kelvin);
	printf("FPA temp kelvin (GetSysFpaTemperatureKelvin) : %i \n", fpa_temp_kelvin); 
	//printf("FPA temp kelvin (GetSysFpaTemperatureKelvin) result já dividido por 100: %i, \n", fpa_temp_kelvin/100, result); 


	LEP_SYS_AUX_TEMPERATURE_KELVIN_T sysauxtemp;
	LEP_GetSysAuxTemperatureKelvin(&_port, &sysauxtemp);
	printf("SysAuxTemp (GetSysAuxTemperatureKelvin): %d \n", sysauxtemp);


	LEP_SYS_AUX_TEMPERATURE_CELCIUS_T sysauxTCelsius;
	LEP_GetSysAuxTemperatureCelcius(&_port, &sysauxTCelsius);
	printf("Temp C AUX(GetSysAuxTemperatureCelcius): %d, %d \n", sysauxTCelsius);



	LEP_SYS_FPA_TEMPERATURE_CELCIUS_T sysfpaTCelsius;
	LEP_GetSysFpaTemperatureCelcius(&_port, &sysfpaTCelsius);
	printf("Temp C FPA(GetSysFpaTemperatureCelcius): %d, %d \n", sysfpaTCelsius);

	//printf("Sys fpa\n");
}





void lepton_ffc_manual() {
	//verify connection
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}
	LEP_SetSysTelemetryEnableState(&_port, LEP_TELEMETRY_ENABLED);
	printf("telemetry enable\n");
		
	//printf("fcc manual\n");
}





void lepton_rad_info() {
	//verify connection
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}



	if (LEP_RunSysPing(&_port)== LEP_OK)
	printf("RunSysPing: %i, %i\n");
	else
	printf("rad info\n");
}



void lepton_ffc_auto() {
	//verify connection
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}
	LEP_AGC_ENABLE_E isAGCEnabled;
//
	LEP_GetAgcEnableState(&_port, &isAGCEnabled);
	printf("LEP_GetAgcEnableState: %d, %i\n", isAGCEnabled);
	


	if (isAGCEnabled != 0){
		printf("AGC : %d\n", isAGCEnabled);
		printf("AGC Enable\n");
	}
		else{
		printf("AGC : %d\n", isAGCEnabled);
		printf("AGC Disable \n");
	}


	//LEP_RAD_ENABLE_E tlinear = LEP_RAD_DISABLE;
	//LEP_GetRadTLinearEnableState(&_port, LEP_RAD_DISABLED);
    	//printf(" %d\n", tlinear);

	printf("FFC AUTO\n");
}





void lepton_disable_tlinear_rad() {
	//verify connection
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}
 

	printf("Disable tlinear rad\n");
}


void lepton_fpa_roi() {
	//verify connection
	if(!_connected) {
		int res = lepton_connect();
		if (res != 0) {
			//check SDA and SCL lines if you get this error
			printf("I2C could not connect\n");
			printf("error code: %d\n", res);
		}
	}
	int res = lepton_connect();
	// Get Lepton region of interest
    LEP_SYS_VIDEO_ROI_T sceneRoi;
    LEP_GetSysSceneRoi(&_port, &sceneRoi);
    printf("Result: %d\n", res);
    printf("Rows: %d - %d \t Columns: %d - %d \n", sceneRoi.startRow, sceneRoi.endRow, sceneRoi.startCol, sceneRoi.endCol);

    // Attempt to set the values of the region of interest...
    printf("Attempting to set the ROI...\n");
    sceneRoi.startRow = 54;
    sceneRoi.startCol = 74;
    sceneRoi.endRow = 64;
    sceneRoi.endCol = 84;
    LEP_SetSysSceneRoi(&_port, sceneRoi);
    printf("Result: %d\n", res);



    // After setting, does it change?
    LEP_GetSysSceneRoi(&_port, &sceneRoi);
    printf("Result: %d\n", res);
    printf("Rows: %d - %d \t Columns: %d - %d \n", sceneRoi.startRow, sceneRoi.endRow, sceneRoi.startCol, sceneRoi.endCol);
    
}



	

