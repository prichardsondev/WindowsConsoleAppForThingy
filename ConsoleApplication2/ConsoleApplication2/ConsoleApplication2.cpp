// ConsoleApplication2.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"

//How to get notification values from a HR BLE monitor
//Ensure that you have paired the HR BLE monitor with the computer

#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bthdef.h>
#include <bluetoothleapis.h>
#include <time.h>
#include <inttypes.h>
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothApis.lib")
// #define TO_SEARCH_DEVICE_UUID "{0000180D-0000-1000-8000-00805F9B34FB}" //we use UUID for an HR BLE device 
#define TO_SEARCH_DEVICE_UUID "{EF680400-9B35-4933-9B10-52FFA9740042}" //

const int MAX_DEVICE_THINGY = 2;

// ULONGLONG TickCount64 = 0;
// set the appropriate callback function when the descriptor change value
// BLUETOOTH_GATT_EVENT_HANDLE EventHandle;
// HANDLE hLEDevice;
// GUID AGuid;

HANDLE                      hLEDeviceThingy[MAX_DEVICE_THINGY];
BLUETOOTH_GATT_EVENT_HANDLE EventHandleThingy[MAX_DEVICE_THINGY];
int                         CallBackValueThingy[MAX_DEVICE_THINGY];

GUID AGuidThingy;

ULONGLONG TickCount64Thingy[MAX_DEVICE_THINGY];

// Thingy  
float                       QwThingy[MAX_DEVICE_THINGY],
							QxThingy[MAX_DEVICE_THINGY],
							QyThingy[MAX_DEVICE_THINGY],
							QzThingy[MAX_DEVICE_THINGY];

// Recuperation des données de la fonction de toficiation 
unsigned char QwB[MAX_DEVICE_THINGY][4];
unsigned char QxB[MAX_DEVICE_THINGY][4];
unsigned char QyB[MAX_DEVICE_THINGY][4];
unsigned char QzB[MAX_DEVICE_THINGY][4];


//
PBTH_LE_GATT_SERVICE							pServiceBuffer[MAX_DEVICE_THINGY];
PBTH_LE_GATT_CHARACTERISTIC						pCharBuffer[MAX_DEVICE_THINGY];
PBTH_LE_GATT_DESCRIPTOR							pDescriptorBuffer[MAX_DEVICE_THINGY];
PBTH_LE_GATT_DESCRIPTOR_VALUE					pDescValueBuffer[MAX_DEVICE_THINGY];
BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterIn[MAX_DEVICE_THINGY];
BTH_LE_GATT_DESCRIPTOR_VALUE					DescriptorValue[MAX_DEVICE_THINGY];



// void OpenBluetooth();
// void CloseBluetooth();

//
void OpenBluetoothThingy(int Index);
void CloseBluetoothThingy(int Index);

float GetFloatWThingy(int Index)
{
	return QwThingy[Index];
}

float GetFloatXThingy(int Index)
{
	return QxThingy[Index];
}

float _stdcall GetFloatYThingy(int Index)
{
	return QyThingy[Index];
}

float GetFloatZThingy(int Index)
{
	return QzThingy[Index];
}



/*
public String ParseMOTThingy(byte[] data)
{
//
String Retour = "";

float SCALE_FACTOR = 1073741824.0F;

//
float qi, qj, qk, qs;

byte[] QiB = new byte[4];
byte[] QjB = new byte[4];
byte[] QkB = new byte[4];
byte[] QsB = new byte[4];


//
QsB[0] = data[0];
QsB[1] = data[1];
QsB[2] = data[2];
QsB[3] = data[3];

qs = (float)(BitConverter.ToInt32(QsB, 0)) / SCALE_FACTOR;

//
QiB[0] = data[4];
QiB[1] = data[5];
QiB[2] = data[6];
QiB[3] = data[7];
qi = (float)(BitConverter.ToInt32(QiB, 0)) / SCALE_FACTOR;

//
QjB[0] = data[8];
QjB[1] = data[9];
QjB[2] = data[10];
QjB[3] = data[11];
qj = (float)(BitConverter.ToInt32(QjB, 0)) / SCALE_FACTOR;

//
QkB[0] = data[12];
QkB[1] = data[13];
QkB[2] = data[14];
QkB[3] = data[15];
qk = (float)(BitConverter.ToInt32(QkB, 0)) / SCALE_FACTOR;


// on calcule qs
// qs = getQs(qi, qj, qk);
rootPage.Q1 = new SharpDX.Quaternion(qi, qj, qk, qs);

// Retour = String.Format("Qi:{0},Qj:{1},Qk:{2},Qs:{3}, Norm {4}", qi, qj, qk, qs, rootPage.Q1.IsNormalized);
// Retour = String.Format("Angle: x:{0},y:{1},z:{2}", rootPage.Q1.Axis.X, rootPage.Q1.Axis.Y, rootPage.Q1.Axis.Z);

// Retour = String.Format("Qi:{0},Qj:{1},Qk:{2},Qs:{3}, Norm {4}",
Retour = String.Format("Qs:{0}, Norm {1}", qs, rootPage.Q1.IsNormalized);


return Retour;
}
*/


//this is the notification function
//the way ValueChangedEventParameters is utilized is shown in
//HealthHeartRateService::HeartRateMeasurementEvent()
//a function in Windows Driver Kit (WDK) 8.0 Samples.zip\C++\WDK 8.0 Samples\Bluetooth Low Energy (LE) Generic Attribute (GATT) Profile Drivers\Solution\WpdHealthHeartRate\HealthHeartRateService.cpp

/*
void CALLBACK SomethingHappened(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
	float SCALE_FACTOR = 1073741824.0F;
	// float SCALE_FACTOR = 65536.0F;
	// 	float Ax, Ay, Az;
	float Qw, Qx, Qy, Qz;
	// unsigned char QxB[4],QyB[4],QzB[4];

	unsigned char QwB[4], QxB[4], QyB[4], QzB[4];

	printf("notification obtained ");
	PBLUETOOTH_GATT_VALUE_CHANGED_EVENT ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

	HRESULT hr;
	if (0x10 != ValueChangedEventParameters->CharacteristicValue->DataSize) {
		hr = E_FAIL;
		printf("datasize 0\n");
	}
	else
	{
		printf("HR ");
		//for(int i=0; i<ValueChangedEventParameters->CharacteristicValue->DataSize;i++) {
		//	printf("%0x",ValueChangedEventParameters->CharacteristicValue->Data[i]);
		//}
		// if the first bit is set, then the value is the next 2 bytes.  If it is clear, the value is in the next byte
		//The Heart Rate Value Format bit (bit 0 of the Flags field) indicates if the data format of 
		//the Heart Rate Measurement Value field is in a format of UINT8 or UINT16. 
		//When the Heart Rate Value format is sent in a UINT8 format, the Heart Rate Value 
		//Format bit shall be set to 0. When the Heart Rate Value format is sent in a UINT16 
		//format, the Heart Rate Value Format bit shall be set to 1
		//from this PDF https://www.bluetooth.org/docman/handlers/downloaddoc.ashx?doc_id=239866
		
		QwB[0] = ValueChangedEventParameters->CharacteristicValue->Data[0];
		QwB[1] = ValueChangedEventParameters->CharacteristicValue->Data[1];
		QwB[2] = ValueChangedEventParameters->CharacteristicValue->Data[2];
		QwB[3] = ValueChangedEventParameters->CharacteristicValue->Data[3];

		// qs = (float)(BitConverter.ToInt32(QsB, 0)) / SCALE_FACTOR;
		int iQwB = ((QwB[3] << 24) & 0xff000000) |
			((QwB[2] << 16) & 0x00ff0000) |
			((QwB[1] << 8) & 0x0000ff00) |
			((QwB[0] << 0) & 0x000000ff);


		QxB[0] = ValueChangedEventParameters->CharacteristicValue->Data[4];
		QxB[1] = ValueChangedEventParameters->CharacteristicValue->Data[5];
		QxB[2] = ValueChangedEventParameters->CharacteristicValue->Data[6];
		QxB[3] = ValueChangedEventParameters->CharacteristicValue->Data[7];

		// qs = (float)(BitConverter.ToInt32(QsB, 0)) / SCALE_FACTOR;
		int iQxB = ((QxB[3] << 24) & 0xff000000) |
			((QxB[2] << 16) & 0x00ff0000) |
			((QxB[1] << 8) & 0x0000ff00) |
			((QxB[0] << 0) & 0x000000ff);

		QyB[0] = ValueChangedEventParameters->CharacteristicValue->Data[8];
		QyB[1] = ValueChangedEventParameters->CharacteristicValue->Data[9];
		QyB[2] = ValueChangedEventParameters->CharacteristicValue->Data[10];
		QyB[3] = ValueChangedEventParameters->CharacteristicValue->Data[11];
		//
		int iQyB = ((QyB[3] << 24) & 0xff000000) |
			((QyB[2] << 16) & 0x00ff0000) |
			((QyB[1] << 8) & 0x0000ff00) |
			((QyB[0] << 0) & 0x000000ff);


		QzB[0] = ValueChangedEventParameters->CharacteristicValue->Data[12];
		QzB[1] = ValueChangedEventParameters->CharacteristicValue->Data[13];
		QzB[2] = ValueChangedEventParameters->CharacteristicValue->Data[14];
		QzB[3] = ValueChangedEventParameters->CharacteristicValue->Data[15];

		//
		int iQzB = ((QzB[3] << 24) & 0xff000000) |
			((QzB[2] << 16) & 0x00ff0000) |
			((QzB[1] << 8) & 0x0000ff00) |
			((QzB[0] << 0) & 0x000000ff);


		// reduction en flottant
		Qw = (float)(iQwB) / SCALE_FACTOR;
		Qx = (float)(iQxB) / SCALE_FACTOR;
		Qy = (float)(iQyB) / SCALE_FACTOR;
		Qz = (float)(iQzB) / SCALE_FACTOR;
		//
		printf("%X,%X,%X,%X\n", iQwB, iQxB, iQyB, iQzB);
		printf("%f,%f,%f,%f\n", Qw, Qx, Qy, Qz);
		//
		ULONGLONG NewTickCount64 = GetTickCount64();
		//
		printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64, NewTickCount64);
		//
		TickCount64 = NewTickCount64;
	}
}
*/

//this is the notification function
void CALLBACK SomethingHappenedThingy(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
	float SCALE_FACTOR = 1073741824.0F;
	printf("notification obtained %d \r\n",*((PINT)Context));
	PBLUETOOTH_GATT_VALUE_CHANGED_EVENT ValueChangedEventParameters = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

	HRESULT hr;
	if (0x10 != ValueChangedEventParameters->CharacteristicValue->DataSize) {
		hr = E_FAIL;
		printf("datasize 0\n");
	}
	else
	{
		// printf("Thingy");
		//
		QwB[*((PINT)Context)][0] = ValueChangedEventParameters->CharacteristicValue->Data[0];
		QwB[*((PINT)Context)][1] = ValueChangedEventParameters->CharacteristicValue->Data[1];
		QwB[*((PINT)Context)][2] = ValueChangedEventParameters->CharacteristicValue->Data[2];
		QwB[*((PINT)Context)][3] = ValueChangedEventParameters->CharacteristicValue->Data[3];
		//
		int iQwB = ((QwB[*((PINT)Context)][3] << 24) & 0xff000000) |
			       ((QwB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
			       ((QwB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
			       ((QwB[*((PINT)Context)][0] << 0) & 0x000000ff);
		//
		QxB[*((PINT)Context)][0] = ValueChangedEventParameters->CharacteristicValue->Data[4];
		QxB[*((PINT)Context)][1] = ValueChangedEventParameters->CharacteristicValue->Data[5];
		QxB[*((PINT)Context)][2] = ValueChangedEventParameters->CharacteristicValue->Data[6];
		QxB[*((PINT)Context)][3] = ValueChangedEventParameters->CharacteristicValue->Data[7];
		//
		int iQxB = ((QxB[*((PINT)Context)][3] << 24) & 0xff000000) |
				   ((QxB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
			       ((QxB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
			       ((QxB[*((PINT)Context)][0] << 0) & 0x000000ff);
		//
		QyB[*((PINT)Context)][0] = ValueChangedEventParameters->CharacteristicValue->Data[8];
		QyB[*((PINT)Context)][1] = ValueChangedEventParameters->CharacteristicValue->Data[9];
		QyB[*((PINT)Context)][2] = ValueChangedEventParameters->CharacteristicValue->Data[10];
		QyB[*((PINT)Context)][3] = ValueChangedEventParameters->CharacteristicValue->Data[11];
		//
		int iQyB = ((QyB[*((PINT)Context)][3] << 24) & 0xff000000) |
				   ((QyB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
			       ((QyB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
			       ((QyB[*((PINT)Context)][0] << 0) & 0x000000ff);
		//
		QzB[*((PINT)Context)][0] = ValueChangedEventParameters->CharacteristicValue->Data[12];
		QzB[*((PINT)Context)][1] = ValueChangedEventParameters->CharacteristicValue->Data[13];
		QzB[*((PINT)Context)][2] = ValueChangedEventParameters->CharacteristicValue->Data[14];
		QzB[*((PINT)Context)][3] = ValueChangedEventParameters->CharacteristicValue->Data[15];
		//
		int iQzB = ((QzB[*((PINT)Context)][3] << 24) & 0xff000000) |
			       ((QzB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
			       ((QzB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
			       ((QzB[*((PINT)Context)][0] << 0) & 0x000000ff);

		// reduction en flottant
		QwThingy[*((PINT)Context)] = (float)(iQwB) / SCALE_FACTOR;
		QxThingy[*((PINT)Context)] = (float)(iQxB) / SCALE_FACTOR;
		QyThingy[*((PINT)Context)] = (float)(iQyB) / SCALE_FACTOR;
		QzThingy[*((PINT)Context)] = (float)(iQzB) / SCALE_FACTOR;
		//
		// printf("%X,%X,%X,%X\n", iQwB, iQxB, iQyB, iQzB);
		// printf("%f,%f,%f,%f\n", QwThingy[*((PINT)Context)], QxThingy[*((PINT)Context)], QyThingy[*((PINT)Context)], QzThingy[*((PINT)Context)]);
		//
		ULONGLONG NewTickCount64 = GetTickCount64();
		//
		//printf("%I64u,%I64u\n,", NewTickCount64 - TickCount64Thingy[*((PINT)Context)], NewTickCount64);
		//
		TickCount64Thingy[*((PINT)Context)] = NewTickCount64;
	}
}


// fonct validé

HANDLE GetBLEHandleOffset(__in GUID AGuid, __in int Index)
{
	HDEVINFO hDI;
	SP_DEVICE_INTERFACE_DATA did;
	SP_DEVINFO_DATA dd;
	GUID BluetoothInterfaceGUID = AGuid;
	HANDLE hComm = NULL;

	hDI = SetupDiGetClassDevs(&BluetoothInterfaceGUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	if (hDI == INVALID_HANDLE_VALUE) return NULL;

	did.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	dd.cbSize = sizeof(SP_DEVINFO_DATA);

	for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDI, NULL, &BluetoothInterfaceGUID, i, &did); i++)
	{
		SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;

		DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		DWORD size = 0;

		if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, NULL, 0, &size, 0))
		{
			int err = GetLastError();

			if (err == ERROR_NO_MORE_ITEMS) break;

			PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);

			pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			if (!SetupDiGetDeviceInterfaceDetail(hDI, &did, pInterfaceDetailData, size, &size, &dd))
				break;

			//
			// On recupere le handle du device qui nous intéresse.
			// Si il n'existe pas ou si il n'est pas dans le range, la valeur sera NULL
			if (i == Index)
			{
				hComm = CreateFile(
					pInterfaceDetailData->DevicePath,
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_OVERLAPPED,
					NULL);
			}

			GlobalFree(pInterfaceDetailData);
		}
	}

	SetupDiDestroyDeviceInfoList(hDI);
	return hComm;
}



int __cdecl  main(int argc, char *argv[], char *envp[])
{

	OpenBluetoothThingy(0);
//	OpenBluetoothThingy(1);


	//go into an inf loop that sleeps. you will ideally see notifications from the HR device
	while (1) {
		Sleep(1000);
		//printf("look for notification");
		// On check si 1 seconde s'est écoulé dpeuis la derniere reception
		LONGLONG CompareTickCount64_1 = GetTickCount64();
		if ((CompareTickCount64_1 - TickCount64Thingy[0]) > 500)
		{
			printf("Timeout");
			// nettoye la connexion
			CloseBluetoothThingy(0);
			OpenBluetoothThingy(0);
		}
		else
		{
			// On affiche les valeurs
			printf("Thingy 0: %f,%f,%f,%f\r\n", GetFloatWThingy(0), GetFloatXThingy(0), GetFloatYThingy(0), GetFloatZThingy(0));
			printf("Thingy 1: %f,%f,%f,%f\r\n", GetFloatWThingy(1), GetFloatXThingy(1), GetFloatYThingy(1), GetFloatZThingy(1));
		}

	////
	////printf("look for notification");
	//// On check si 1 seconde s'est écoulé dpeuis la derniere reception
	//	LONGLONG CompareTickCount64_2 = GetTickCount64();
	//	if ((CompareTickCount64_2 - TickCount64Thingy[1]) > 2000)
	//	{
	//		printf("Timeout");
	//		// nettoye la connexion
	//		CloseBluetoothThingy(1);
	//		OpenBluetoothThingy(1);
	//	}
	//	else
	//	{
	//		// On affiche les valeurs
	//		printf("Thingy 1: %f,%f,%f,%f\r\n", GetFloatWThingy(1), GetFloatXThingy(1), GetFloatYThingy(1), GetFloatZThingy(1));
	//	}
	}

	CloseBluetoothThingy(0);
//	CloseBluetoothThingy(1);
	

	if (GetLastError() != NO_ERROR &&
		GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		// Insert error handling here.
		return 1;
	}

	return 0;
}

//
void CloseBluetoothThingy(int Index)
{
	BluetoothGATTUnregisterEvent(EventHandleThingy[Index], BLUETOOTH_GATT_FLAG_NONE);
	CloseHandle(hLEDeviceThingy[Index]);
}


void OpenBluetoothThingy(int Index)
{
	//GUID can be constructed from "{xxx....}" string using CLSID
	CLSIDFromString(TEXT(TO_SEARCH_DEVICE_UUID), &AGuidThingy);

	//now get the handle 
	hLEDeviceThingy[Index] = GetBLEHandleOffset(AGuidThingy, Index);

	//
	TickCount64Thingy[Index] = GetTickCount64();

	//Step 2: Get a list of services that the device advertises
	// first send 0,NULL as the parameters to BluetoothGATTServices inorder to get the number of
	// services in serviceBufferCount
	USHORT serviceBufferCount;
	////////////////////////////////////////////////////////////////////////////
	// Determine Services Buffer Size
	////////////////////////////////////////////////////////////////////////////

	HRESULT hr = BluetoothGATTGetServices(hLEDeviceThingy[Index],0,NULL,&serviceBufferCount,BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
		printf("BluetoothGATTGetServices - Buffer Size %d", hr);
	}

	// pServiceBuffer[Index] = (PBTH_LE_GATT_SERVICE)malloc(sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
	pServiceBuffer[Index] = (PBTH_LE_GATT_SERVICE)GlobalAlloc(GPTR,sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);

	if (NULL == pServiceBuffer[Index]) {
		printf("pServiceBuffer out of memory\r\n");
	}
	else {
		RtlZeroMemory(pServiceBuffer[Index],
			sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
	}

	////////////////////////////////////////////////////////////////////////////
	// Retrieve Services
	////////////////////////////////////////////////////////////////////////////

	USHORT numServices;
	hr = BluetoothGATTGetServices(hLEDeviceThingy[Index],serviceBufferCount,pServiceBuffer[Index],&numServices,BLUETOOTH_GATT_FLAG_NONE);
	//
	if (S_OK != hr) {
		printf("BluetoothGATTGetServices - Buffer Size %d", hr);
	}


	//Step 3: now get the list of charactersitics. note how the pServiceBuffer[Index] is required from step 2
	////////////////////////////////////////////////////////////////////////////
	// Determine Characteristic Buffer Size
	////////////////////////////////////////////////////////////////////////////

	USHORT charBufferSize;
	hr = BluetoothGATTGetCharacteristics(hLEDeviceThingy[Index],pServiceBuffer[Index],0,NULL,&charBufferSize,BLUETOOTH_GATT_FLAG_NONE);

	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
		printf("BluetoothGATTGetCharacteristics - Buffer Size %d", hr);
	}

	pCharBuffer[Index] = NULL;

	if (charBufferSize > 0) {
		// pCharBuffer[Index] = (PBTH_LE_GATT_CHARACTERISTIC) malloc(charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
		pCharBuffer[Index] = (PBTH_LE_GATT_CHARACTERISTIC) GlobalAlloc(GPTR, charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));


		if (NULL == pCharBuffer[Index]) {
			printf("pCharBuffer out of memory\r\n");
		}
		else {
			RtlZeroMemory(pCharBuffer[Index],charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
		}

		////////////////////////////////////////////////////////////////////////////
		// Retrieve Characteristics
		////////////////////////////////////////////////////////////////////////////
		USHORT numChars;
		hr = BluetoothGATTGetCharacteristics(hLEDeviceThingy[Index],pServiceBuffer[Index],charBufferSize,pCharBuffer[Index],&numChars,BLUETOOTH_GATT_FLAG_NONE);

		if (S_OK != hr) {
			printf("BluetoothGATTGetCharacteristics - Actual Data %d", hr);
		}

		if (numChars != charBufferSize) {
			printf("buffer size and buffer size actual size mismatch\r\n");
		}
	}


	//Step 4: now get the list of descriptors. note how the pCharBuffer is required from step 3
	//descriptors are required as we descriptors that are notification based will have to be written
	//once IsSubcribeToNotification set to true, we set the appropriate callback function
	//need for setting descriptors for notification according to
	//http://social.msdn.microsoft.com/Forums/en-US/11d3a7ce-182b-4190-bf9d-64fefc3328d9/windows-bluetooth-le-apis-event-callbacks?forum=wdk
	PBTH_LE_GATT_CHARACTERISTIC currGattChar;
	for (int ii = 0; ii <charBufferSize; ii++) {
		//
		// 
		currGattChar = &pCharBuffer[Index][ii];
		//
		USHORT charValueDataSize;
		PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer;


		///////////////////////////////////////////////////////////////////////////
		// Determine Descriptor Buffer Size
		////////////////////////////////////////////////////////////////////////////
		USHORT descriptorBufferSize;
		hr = BluetoothGATTGetDescriptors(hLEDeviceThingy[Index],currGattChar,0,NULL,&descriptorBufferSize,BLUETOOTH_GATT_FLAG_NONE);

		if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {printf("BluetoothGATTGetDescriptors - Buffer Size %d", hr);}

		if (descriptorBufferSize > 0) {
			// pDescriptorBuffer[Index] = (PBTH_LE_GATT_DESCRIPTOR) malloc(descriptorBufferSize* sizeof(BTH_LE_GATT_DESCRIPTOR));
			pDescriptorBuffer[Index] = (PBTH_LE_GATT_DESCRIPTOR) GlobalAlloc(GPTR, descriptorBufferSize * sizeof(BTH_LE_GATT_DESCRIPTOR));

			if (NULL == pDescriptorBuffer[Index]) {printf("pDescriptorBuffer out of memory\r\n");}
			else 
			{
			RtlZeroMemory(pDescriptorBuffer[Index], descriptorBufferSize);
			}

			////////////////////////////////////////////////////////////////////////////
			// Retrieve Descriptors
			////////////////////////////////////////////////////////////////////////////

			USHORT numDescriptors;
			hr = BluetoothGATTGetDescriptors(hLEDeviceThingy[Index],currGattChar,descriptorBufferSize,pDescriptorBuffer[Index],&numDescriptors,BLUETOOTH_GATT_FLAG_NONE);

			if (S_OK != hr) {
				printf("BluetoothGATTGetDescriptors - Actual Data %d", hr);
			}

			if (numDescriptors != descriptorBufferSize) {
				printf("buffer size and buffer size actual size mismatch\r\n");
			}

			for (int kk = 0; kk<numDescriptors; kk++) {
				// Recuperation du pointeur
				PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBuffer[Index][kk];
				////////////////////////////////////////////////////////////////////////////
				// Determine Descriptor Value Buffer Size
				////////////////////////////////////////////////////////////////////////////
				USHORT descValueDataSize;
				hr = BluetoothGATTGetDescriptorValue(	hLEDeviceThingy[Index],currGattDescriptor,0,NULL,&descValueDataSize,BLUETOOTH_GATT_FLAG_NONE);

				if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
					printf("BluetoothGATTGetDescriptorValue - Buffer Size %d", hr);
				}

				// pDescValueBuffer[Index] = (PBTH_LE_GATT_DESCRIPTOR_VALUE)malloc(descValueDataSize);
				pDescValueBuffer[Index] = (PBTH_LE_GATT_DESCRIPTOR_VALUE)GlobalAlloc(GPTR, descValueDataSize);
				//
				if (NULL == pDescValueBuffer[Index]) {printf("pDescValueBuffer out of memory\r\n");}
				else {
					RtlZeroMemory(pDescValueBuffer[Index], descValueDataSize);
				}

				////////////////////////////////////////////////////////////////////////////
				// Retrieve the Descriptor Value
				////////////////////////////////////////////////////////////////////////////

				hr = BluetoothGATTGetDescriptorValue(hLEDeviceThingy[Index],currGattDescriptor,	(ULONG)descValueDataSize, pDescValueBuffer[Index],NULL,	BLUETOOTH_GATT_FLAG_NONE);
				if (S_OK != hr) {
					printf("BluetoothGATTGetDescriptorValue - Actual Data %d", hr);
				}
				//you may also get a descriptor that is read (and not notify) andi am guessing the attribute handle is out of limits
				// we set all descriptors that are notifiable to notify us via IsSubstcibeToNotification
				if ((currGattDescriptor->AttributeHandle < 255) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x404))
				{
					// Creation d'une nouvelle structure
					RtlZeroMemory(&DescriptorValue[Index], sizeof(BTH_LE_GATT_DESCRIPTOR_VALUE));
					
					// Initialisattion de la structure
					DescriptorValue[Index].DescriptorType = ClientCharacteristicConfiguration;
					DescriptorValue[Index].ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;

					hr = BluetoothGATTSetDescriptorValue(hLEDeviceThingy[Index],currGattDescriptor,&DescriptorValue[Index],BLUETOOTH_GATT_FLAG_NONE);
					if (S_OK != hr) {
						printf("BluetoothGATTGetDescriptorValue - Actual Data %d\n", hr);
					}
					else {
						printf("setting notification for serivice handle %d\n", currGattDescriptor->ServiceHandle);
					}
				}
			}
		}


		if ((currGattChar->IsNotifiable) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x404))
		{
			printf("Setting Notification for ServiceHandle %d --- %X \n", currGattChar->ServiceHandle, currGattChar->CharacteristicUuid.Value.ShortUuid);
			BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;
			// EventParameterIn.Characteristics[0] = *currGattChar;
			memcpy(EventParameterIn[Index].Characteristics, currGattChar, sizeof(BTH_LE_GATT_CHARACTERISTIC));
			EventParameterIn[Index].NumCharacteristics = 1;
			//
			CallBackValueThingy[Index] = Index;
			//
			hr = BluetoothGATTRegisterEvent(hLEDeviceThingy[Index],EventType,&EventParameterIn[Index],SomethingHappenedThingy,&CallBackValueThingy[Index],&EventHandleThingy[Index],BLUETOOTH_GATT_FLAG_NONE);

			if (S_OK != hr) {
				printf("BluetoothGATTRegisterEvent - Actual Data %d", hr);
			}
		}
	}
}
