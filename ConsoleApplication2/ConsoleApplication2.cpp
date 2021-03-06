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
#define TO_SEARCH_DEVICE_UUID_MOTION "{EF680400-9B35-4933-9B10-52FFA9740042}" //
#define TO_SEARCH_DEVICE_UUID_UI "{EF680300-9B35-4933-9B10-52FFA9740042}" //


const int MAX_DEVICE_THINGY = 2;

// ULONGLONG TickCount64 = 0;
// set the appropriate callback function when the descriptor change value
// BLUETOOTH_GATT_EVENT_HANDLE EventHandle;
// HANDLE hLEDevice;
// GUID AGuid;

/*
class ThingyMotion
{
*/


	GUID						AGuidThingyMotion;
	HANDLE                      hLEDeviceThingyMotion[MAX_DEVICE_THINGY];
	BLUETOOTH_GATT_EVENT_HANDLE EventHandleThingyMotion[MAX_DEVICE_THINGY];
	int                         CallBackValueThingyMotion[MAX_DEVICE_THINGY];

	GUID						AGuidThingyUI;
	HANDLE                      hLEDeviceThingyUI[MAX_DEVICE_THINGY];
	BLUETOOTH_GATT_EVENT_HANDLE EventHandleThingyUI[MAX_DEVICE_THINGY];
	int                         CallBackValueThingyUI[MAX_DEVICE_THINGY];
	
	USHORT serviceBufferCountMotion[MAX_DEVICE_THINGY] = { 0,0 };
	USHORT CharacteristicBufferCountMotion[MAX_DEVICE_THINGY] = { 0,0 };
	USHORT descriptorBufferCountMotion[MAX_DEVICE_THINGY] = { 0,0 };
	
	USHORT serviceBufferCountUI[MAX_DEVICE_THINGY] = { 0,0 };
	USHORT CharacteristicBufferCountUI[MAX_DEVICE_THINGY] = { 0,0 };
	USHORT descriptorBufferCountUI[MAX_DEVICE_THINGY] = { 0,0 };


	PBTH_LE_GATT_SERVICE							pServiceBufferMotion[MAX_DEVICE_THINGY];
	PBTH_LE_GATT_CHARACTERISTIC						pCharBufferMotion[MAX_DEVICE_THINGY];
	PBTH_LE_GATT_DESCRIPTOR							pDescriptorBufferMotion[MAX_DEVICE_THINGY];
	
	//
	PBTH_LE_GATT_SERVICE							pServiceBufferUI[MAX_DEVICE_THINGY];
	PBTH_LE_GATT_CHARACTERISTIC						pCharBufferUI[MAX_DEVICE_THINGY];
	PBTH_LE_GATT_DESCRIPTOR							pDescriptorBufferUI[MAX_DEVICE_THINGY];


	// PBTH_LE_GATT_DESCRIPTOR_VALUE					pDescValueBuffer[MAX_DEVICE_THINGY];


	BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterInMotion[MAX_DEVICE_THINGY];
	BTH_LE_GATT_DESCRIPTOR_VALUE					DescriptorValueMotion[MAX_DEVICE_THINGY];

	BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION EventParameterInUI[MAX_DEVICE_THINGY];
	BTH_LE_GATT_DESCRIPTOR_VALUE					DescriptorValueUI[MAX_DEVICE_THINGY];
		
// public:
	ULONGLONG TickCount64ThingyMotion[MAX_DEVICE_THINGY];
	ULONGLONG TickCount64ThingyUI[MAX_DEVICE_THINGY];
	// Thingy  
	float	QwThingy[MAX_DEVICE_THINGY],
			QxThingy[MAX_DEVICE_THINGY],
			QyThingy[MAX_DEVICE_THINGY],
			QzThingy[MAX_DEVICE_THINGY];
	
	// Recuperation des données de la fonction de toficiation 
	unsigned char QwB[MAX_DEVICE_THINGY][4];
	unsigned char QxB[MAX_DEVICE_THINGY][4];
	unsigned char QyB[MAX_DEVICE_THINGY][4];
	unsigned char QzB[MAX_DEVICE_THINGY][4];
	
	static PBLUETOOTH_GATT_VALUE_CHANGED_EVENT				ValueChangedEventParametersMotion[MAX_DEVICE_THINGY];
	static PBLUETOOTH_GATT_VALUE_CHANGED_EVENT				ValueChangedEventParametersUI[MAX_DEVICE_THINGY];


	// Creation d'un tableau de pointeur de fonction pour dupliquer les appel ?
	typedef void (CALLBACK *CallList)(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context);
	void readCharacteristic(int Index);
	void WriteCharacteristic(int Index, UCHAR ENABLE, UCHAR RED, UCHAR GREEN, UCHAR BLUE);


	
	//this is the notification function
	void CALLBACK MotionCallBackThingy1(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
	{
		float SCALE_FACTOR = 1073741824.0F;
		// printf("notification obtained %d \r\n",*((PINT)Context));
		ValueChangedEventParametersMotion[*((PINT)Context)] = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

		HRESULT hr;
		if (0x10 != ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->DataSize) {
			hr = E_FAIL;
			printf("datasize 0\n");
		}
		else
		{
			// printf("Thingy");
			//
			QwB[*((PINT)Context)][0] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[0];
			QwB[*((PINT)Context)][1] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[1];
			QwB[*((PINT)Context)][2] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[2];
			QwB[*((PINT)Context)][3] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[3];
			//
			int iQwB = ((QwB[*((PINT)Context)][3] << 24) & 0xff000000) |
				((QwB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
				((QwB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
				((QwB[*((PINT)Context)][0] << 0) & 0x000000ff);
			//
			QxB[*((PINT)Context)][0] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[4];
			QxB[*((PINT)Context)][1] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[5];
			QxB[*((PINT)Context)][2] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[6];
			QxB[*((PINT)Context)][3] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[7];
			//
			int iQxB = ((QxB[*((PINT)Context)][3] << 24) & 0xff000000) |
				((QxB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
				((QxB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
				((QxB[*((PINT)Context)][0] << 0) & 0x000000ff);
			//
			QyB[*((PINT)Context)][0] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[8];
			QyB[*((PINT)Context)][1] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[9];
			QyB[*((PINT)Context)][2] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[10];
			QyB[*((PINT)Context)][3] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[11];
			//
			int iQyB = ((QyB[*((PINT)Context)][3] << 24) & 0xff000000) |
				((QyB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
				((QyB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
				((QyB[*((PINT)Context)][0] << 0) & 0x000000ff);
			//
			QzB[*((PINT)Context)][0] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[12];
			QzB[*((PINT)Context)][1] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[13];
			QzB[*((PINT)Context)][2] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[14];
			QzB[*((PINT)Context)][3] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[15];
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
			TickCount64ThingyMotion[*((PINT)Context)] = GetTickCount64();
		}
	}
	//this is the notification function
	void CALLBACK MotionCallBackThingy2(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
	{
		float SCALE_FACTOR = 1073741824.0F;
		// printf("notification obtained %d \r\n", *((PINT)Context));
		ValueChangedEventParametersMotion[*((PINT)Context)] = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

		HRESULT hr;
		if (0x10 != ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->DataSize) {
			hr = E_FAIL;
			printf("datasize 0\n");
		}
		else
		{
			// printf("Thingy");
			//
			QwB[*((PINT)Context)][0] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[0];
			QwB[*((PINT)Context)][1] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[1];
			QwB[*((PINT)Context)][2] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[2];
			QwB[*((PINT)Context)][3] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[3];
			//
			int iQwB = ((QwB[*((PINT)Context)][3] << 24) & 0xff000000) |
				((QwB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
				((QwB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
				((QwB[*((PINT)Context)][0] << 0) & 0x000000ff);
			//
			QxB[*((PINT)Context)][0] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[4];
			QxB[*((PINT)Context)][1] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[5];
			QxB[*((PINT)Context)][2] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[6];
			QxB[*((PINT)Context)][3] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[7];
			//
			int iQxB = ((QxB[*((PINT)Context)][3] << 24) & 0xff000000) |
				((QxB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
				((QxB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
				((QxB[*((PINT)Context)][0] << 0) & 0x000000ff);
			//
			QyB[*((PINT)Context)][0] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[8];
			QyB[*((PINT)Context)][1] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[9];
			QyB[*((PINT)Context)][2] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[10];
			QyB[*((PINT)Context)][3] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[11];
			//
			int iQyB = ((QyB[*((PINT)Context)][3] << 24) & 0xff000000) |
				((QyB[*((PINT)Context)][2] << 16) & 0x00ff0000) |
				((QyB[*((PINT)Context)][1] << 8) & 0x0000ff00) |
				((QyB[*((PINT)Context)][0] << 0) & 0x000000ff);
			//
			QzB[*((PINT)Context)][0] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[12];
			QzB[*((PINT)Context)][1] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[13];
			QzB[*((PINT)Context)][2] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[14];
			QzB[*((PINT)Context)][3] = ValueChangedEventParametersMotion[*((PINT)Context)]->CharacteristicValue->Data[15];
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
			TickCount64ThingyMotion[*((PINT)Context)] = GetTickCount64();
		}
	}

	
	CallList MotionCallBackThingy[MAX_DEVICE_THINGY]=
	{
	MotionCallBackThingy1,
	MotionCallBackThingy2
	};

	//this is the notification function
void CALLBACK UICallBackThingy1(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
	// printf("notification obtained %d \r\n",*((PINT)Context));
	ValueChangedEventParametersUI[*((PINT)Context)] = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

	HRESULT hr;
	if (0x01 != ValueChangedEventParametersUI[*((PINT)Context)]->CharacteristicValue->DataSize) {
		hr = E_FAIL;
		printf("datasize 0\n");
	}
	else
	{
		printf("THingy 1 Event");
		TickCount64ThingyUI[*((PINT)Context)] = GetTickCount64();
		printf("%u\r\n", (unsigned int)ValueChangedEventParametersUI[*((PINT)Context)]->CharacteristicValue->Data[0]);
		//
		if(ValueChangedEventParametersUI[*((PINT)Context)]->CharacteristicValue->Data[0]==1)
			WriteCharacteristic(*((PINT)Context), 0x01, 0xFF, 0xFF, 0xFF);
		else
			WriteCharacteristic(*((PINT)Context), 0x01, 0x00, 0x00, 0x00);
	}
}

//this is the notification function
void CALLBACK UICallBackThingy2(BTH_LE_GATT_EVENT_TYPE EventType, PVOID EventOutParameter, PVOID Context)
{
	// printf("notification obtained %d \r\n",*((PINT)Context));
	ValueChangedEventParametersUI[*((PINT)Context)] = (PBLUETOOTH_GATT_VALUE_CHANGED_EVENT)EventOutParameter;

	HRESULT hr;
	if (0x01 != ValueChangedEventParametersUI[*((PINT)Context)]->CharacteristicValue->DataSize) {
		hr = E_FAIL;
		printf("datasize 0\n");
	}
	else
	{
		printf("THingy 2 Event");
		TickCount64ThingyUI[*((PINT)Context)] = GetTickCount64();
		printf("%u\r\n", (unsigned int)ValueChangedEventParametersUI[*((PINT)Context)]->CharacteristicValue->Data[0]);

	}
}



CallList UICallBackThingy[MAX_DEVICE_THINGY] =
{
UICallBackThingy1,
UICallBackThingy2
};


	/*
	// creation du consturcteur
	ThingyMotion()
	{
	// initialisation des pointeurs
	}
	*/
	
	float GetFloatWThingy(int Index)
	{
		return QwThingy[Index];
	}
	float GetFloatXThingy(int Index)
	{
		return QxThingy[Index];
	}
	float GetFloatYThingy(int Index)
	{
		return QyThingy[Index];
	}
	float GetFloatZThingy(int Index)
	{
		return QzThingy[Index];
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
	// recherche des services, caractéristiques et descripteurs 
	HRESULT GetBluetoothServices(HANDLE hLEDeviceInformation, PBTH_LE_GATT_SERVICE	*pServiceInformationBuffer, PUSHORT pServiceBufferCount)
	{

		HRESULT hr;
		//
		hr = BluetoothGATTGetServices(hLEDeviceInformation, 0, NULL, pServiceBufferCount, BLUETOOTH_GATT_FLAG_NONE);
		if (hr != S_OK) { printf("BluetoothGATTGetServices: Error during initialisation\r\n"); }

		(*pServiceInformationBuffer) = (PBTH_LE_GATT_SERVICE)GlobalAlloc(GPTR, sizeof(BTH_LE_GATT_SERVICE) * (*pServiceBufferCount));

		if (NULL == (*pServiceInformationBuffer))
		{
			printf("Error memory allocation\r\n");
		}
		else
		{
			RtlZeroMemory((*pServiceInformationBuffer), sizeof(BTH_LE_GATT_SERVICE) * (*pServiceBufferCount));
			////////////////////////////////////////////////////////////////////////////
			// Retrieve Services
			////////////////////////////////////////////////////////////////////////////

			USHORT numServices;
			hr = BluetoothGATTGetServices(hLEDeviceInformation, (*pServiceBufferCount), *pServiceInformationBuffer, &numServices, BLUETOOTH_GATT_FLAG_NONE);
			if (hr != S_OK)
			{
				printf("BluetoothGATTGetServices: Error When obtening information\r\n");
			}
			if (numServices != (*pServiceBufferCount))
			{
				printf("buffer size and buffer size actual size mismatch\r\n");
			}

		}
		// valeur de retour
		return hr;
	}
	HRESULT GetBluetoothCharacteristics(HANDLE hLEDeviceInformation, PBTH_LE_GATT_SERVICE	pServiceInformationBuffer, PBTH_LE_GATT_CHARACTERISTIC *pCharacteristicInformationBuffer, PUSHORT pCharacteristicBufferCount)
	{
		HRESULT hr;

		hr = BluetoothGATTGetCharacteristics(hLEDeviceInformation, pServiceInformationBuffer, 0, NULL, pCharacteristicBufferCount, BLUETOOTH_GATT_FLAG_NONE);
		if (hr != S_OK) { printf("BluetoothGATTGetCharacteristics: Error during initialisation\r\n"); }
		//
		if ((*pCharacteristicBufferCount) > 0)
		{
			(*pCharacteristicInformationBuffer) = (PBTH_LE_GATT_CHARACTERISTIC)GlobalAlloc(GPTR, (*pCharacteristicBufferCount) * sizeof(BTH_LE_GATT_CHARACTERISTIC));
			//
			if (NULL == (*pCharacteristicInformationBuffer))
			{
				printf("Error memory allocation\r\n");
			}
			else
			{
				RtlZeroMemory((*pCharacteristicInformationBuffer), (*pCharacteristicBufferCount) * sizeof(BTH_LE_GATT_CHARACTERISTIC));

				////////////////////////////////////////////////////////////////////////////
				// Retrieve Characteristics
				////////////////////////////////////////////////////////////////////////////
				USHORT numChars;
				hr = BluetoothGATTGetCharacteristics(hLEDeviceInformation, pServiceInformationBuffer, (*pCharacteristicBufferCount), (*pCharacteristicInformationBuffer), &numChars, BLUETOOTH_GATT_FLAG_NONE);
				if (hr != S_OK) { printf("BluetoothGATTGetCharacteristics: Error When obtening information\r\n"); }
				if (numChars != (*pCharacteristicBufferCount))
				{
					printf("buffer size and buffer size actual size mismatch\r\n");
				}
			}
		}
		// valeur de retour
		return hr;
	}
	HRESULT GetBluetoothDescriptors(HANDLE hLEDeviceInformation, PBTH_LE_GATT_CHARACTERISTIC currGattChar, PBTH_LE_GATT_DESCRIPTOR *pDescriptorInformationBuffer, PUSHORT pDescriptorBufferCount)
	{
		HRESULT hr;

		hr = BluetoothGATTGetDescriptors(hLEDeviceInformation, currGattChar, 0, NULL, pDescriptorBufferCount, BLUETOOTH_GATT_FLAG_NONE);
		if (hr != S_OK) { printf("BluetoothGATTGetDescriptors: Error When obtening information\r\n"); }


		if ((*pDescriptorBufferCount) > 0)
		{
			(*pDescriptorInformationBuffer) = (PBTH_LE_GATT_DESCRIPTOR)GlobalAlloc(GPTR, (*pDescriptorBufferCount) * sizeof(BTH_LE_GATT_DESCRIPTOR));

			if (NULL == (*pDescriptorInformationBuffer))
			{
				printf("Error memory allocation\r\n");
			}
			else
			{
				RtlZeroMemory((*pDescriptorInformationBuffer), sizeof(PBTH_LE_GATT_DESCRIPTOR) * (*pDescriptorBufferCount));

				////////////////////////////////////////////////////////////////////////////
				// Retrieve Descriptors
				////////////////////////////////////////////////////////////////////////////

				USHORT numDescriptors;
				hr = BluetoothGATTGetDescriptors(hLEDeviceInformation, currGattChar, (*pDescriptorBufferCount), (*pDescriptorInformationBuffer), &numDescriptors, BLUETOOTH_GATT_FLAG_NONE);
				if (hr != S_OK) { printf("BluetoothGATTGetDescriptors: Error When obtening information\r\n"); }

				if (numDescriptors != (*pDescriptorBufferCount))
				{
					printf("buffer size and buffer size actual size mismatch\r\n");
				}

			}
		}
		// valeur de retour
		return hr;
	}
	HRESULT EnableNotification(HANDLE hLEDeviceInformation, PBTH_LE_GATT_DESCRIPTOR_VALUE DescriptorValue,PBTH_LE_GATT_DESCRIPTOR  *currGattDescriptor)
	{
		HRESULT hr;
		
		// Creation d'une nouvelle structure
		RtlZeroMemory(DescriptorValue, sizeof(BTH_LE_GATT_DESCRIPTOR_VALUE));

		// Initialisattion de la structure
		DescriptorValue->DescriptorType = ClientCharacteristicConfiguration;
		DescriptorValue->ClientCharacteristicConfiguration.IsSubscribeToNotification = TRUE;

		hr = BluetoothGATTSetDescriptorValue(hLEDeviceInformation, (*currGattDescriptor), DescriptorValue, BLUETOOTH_GATT_FLAG_NONE);
		if (S_OK != hr) {
			printf("BluetoothGATTGetDescriptorValue - Actual Data %d\n", hr);
		}
		else {
			printf("setting notification for serivice handle %d\n", (*currGattDescriptor)->ServiceHandle);
		}
		return(hr);
	}

	HRESULT AttachNotification(		HANDLE hLEDeviceInformation, PBTH_LE_GATT_CHARACTERISTIC *currGattChar, BLUETOOTH_GATT_VALUE_CHANGED_EVENT_REGISTRATION *EventParameterInUI, 
									BTH_LE_GATT_EVENT_TYPE EventType, CallList *CallBack, int *CallBackValue, BLUETOOTH_GATT_EVENT_HANDLE *EventHandle)
	{
		//
		HRESULT hr=NULL;
		
		printf("Setting Notification for ServiceHandle in function %d --- %X \n", (*currGattChar)->ServiceHandle, (*currGattChar)->CharacteristicUuid.Value.ShortUuid);
		// Init des valeurs
		//
		memcpy((*EventParameterInUI).Characteristics, *currGattChar, sizeof(BTH_LE_GATT_CHARACTERISTIC));
		(*EventParameterInUI).NumCharacteristics = 1;

		//
		hr = BluetoothGATTRegisterEvent(hLEDeviceInformation, EventType,
			EventParameterInUI, (*CallBack),
			CallBackValue, EventHandle,
			BLUETOOTH_GATT_FLAG_NONE);


		//
		return(hr);
	}
	
	// liberation memoire
	void FreeBluetoothServices(PBTH_LE_GATT_SERVICE	*pServiceInformationBuffer)
	{
		GlobalFree(*pServiceInformationBuffer);
	}
	void FreeBluetoothCharacteristics(PBTH_LE_GATT_CHARACTERISTIC *pCharacteristicInformationBuffer)
	{
		GlobalFree(*pCharacteristicInformationBuffer);
	}
	void FreeBluetoothDescriptors(PBTH_LE_GATT_DESCRIPTOR *pDescriptorInformationBuffer)
	{
		GlobalFree(*pDescriptorInformationBuffer);
	}


	void OpenBluetoothThingyMotion(int Index)
	{
		//GUID can be constructed from "{xxx....}" string using CLSID
		CLSIDFromString(TEXT(TO_SEARCH_DEVICE_UUID_MOTION), &AGuidThingyMotion);

		//
		HRESULT hr;

		serviceBufferCountMotion[Index] = 0;
		CharacteristicBufferCountMotion[Index] = 0;
		descriptorBufferCountMotion[Index] = 0;

		//now get the handle 
		hLEDeviceThingyMotion[Index] = GetBLEHandleOffset(AGuidThingyMotion, Index);

		//
		TickCount64ThingyMotion[Index] = GetTickCount64();

		//Step 2: Get a list of services that the device advertises
		hr = GetBluetoothServices(hLEDeviceThingyMotion[Index], &pServiceBufferMotion[Index], &serviceBufferCountMotion[Index]);

		if ((hr == S_OK) && (pServiceBufferMotion[Index] != NULL))
		{
			//Step 3: now get the list of charactersitics. note how the pServiceBuffer[Index] is required from step 2
			hr = GetBluetoothCharacteristics(hLEDeviceThingyMotion[Index], pServiceBufferMotion[Index], &pCharBufferMotion[Index], &CharacteristicBufferCountMotion[Index]);
			if ((hr == S_OK) && (pCharBufferMotion[Index] != NULL))
			{
				//Step 4: now get the list of descriptors. note how the pCharBuffer is required from step 3
				PBTH_LE_GATT_CHARACTERISTIC currGattChar;
				for (int ii = 0; ii < CharacteristicBufferCountMotion[Index]; ii++)
				{
					// 
					currGattChar = &pCharBufferMotion[Index][ii];
					//
					hr = GetBluetoothDescriptors(hLEDeviceThingyMotion[Index], currGattChar, &pDescriptorBufferMotion[Index], &descriptorBufferCountMotion[Index]);
					if ((hr == S_OK) && (pDescriptorBufferMotion[Index] != NULL))
					{
						for (int kk = 0; kk < descriptorBufferCountMotion[Index]; kk++)
						{
							// Recuperation du pointeur
							PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBufferMotion[Index][kk];
							//
							if ((currGattDescriptor->AttributeHandle < 255) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x404))
							{
								HRESULT ret = EnableNotification(hLEDeviceThingyMotion[Index], &DescriptorValueMotion[Index], &currGattDescriptor);
							}
						}

						// Activation des notifications
						if ((currGattChar->IsNotifiable) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x404))
						{
							// On initialise une variable qui portera le numéro du thingy tracé.
							CallBackValueThingyMotion[Index] = Index;
							// Et le type d'event à surveiller	
							BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;
							//
							HRESULT ret2 = AttachNotification(
								hLEDeviceThingyMotion[Index], &currGattChar,
								&EventParameterInMotion[Index], EventType, &MotionCallBackThingy[Index],
								&CallBackValueThingyMotion[Index], &EventHandleThingyMotion[Index]);
						}
						// Libération de la memoire
						FreeBluetoothDescriptors(&pDescriptorBufferMotion[Index]);
					}
				}
			}
		}
	}

	void OpenBluetoothThingyUI(int Index)
	{
		//GUID can be constructed from "{xxx....}" string using CLSID
		CLSIDFromString(TEXT(TO_SEARCH_DEVICE_UUID_UI), &AGuidThingyUI);

		//
		HRESULT hr;

		serviceBufferCountUI[Index] = 0;
		CharacteristicBufferCountUI[Index] = 0;
		descriptorBufferCountUI[Index] = 0;


		//now get the handle 
		hLEDeviceThingyUI[Index] = GetBLEHandleOffset(AGuidThingyUI, Index);

		//
		TickCount64ThingyUI[Index] = GetTickCount64();

		//Step 2: Get a list of services that the device advertises
		hr = GetBluetoothServices(hLEDeviceThingyUI[Index], &pServiceBufferUI[Index], &serviceBufferCountUI[Index]);

		if ((hr == S_OK) && (pServiceBufferUI[Index] != NULL))
		{
			//Step 3: now get the list of charactersitics. note how the pServiceBuffer[Index] is required from step 2
			hr = GetBluetoothCharacteristics(hLEDeviceThingyUI[Index], pServiceBufferUI[Index], &pCharBufferUI[Index], &CharacteristicBufferCountUI[Index]);
			if ((hr == S_OK) && (pCharBufferUI[Index] != NULL))
			{
				//Step 4: now get the list of descriptors. note how the pCharBuffer is required from step 3
				PBTH_LE_GATT_CHARACTERISTIC currGattChar;
				for (int ii = 0; ii < CharacteristicBufferCountUI[Index]; ii++)
				{
				// 
				currGattChar = &pCharBufferUI[Index][ii];
				//
				hr = GetBluetoothDescriptors(hLEDeviceThingyUI[Index], currGattChar, &pDescriptorBufferUI[Index], &descriptorBufferCountUI[Index]);
				if ((hr == S_OK) && (pDescriptorBufferUI[Index] != NULL))
					{
					for (int kk = 0; kk < descriptorBufferCountUI[Index]; kk++)
						{
							// Recuperation du pointeur
							PBTH_LE_GATT_DESCRIPTOR  currGattDescriptor = &pDescriptorBufferUI[Index][kk];
							//
							if ((currGattDescriptor->AttributeHandle < 255) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x302))
							{
								HRESULT ret = EnableNotification(hLEDeviceThingyUI[Index], &DescriptorValueUI[Index], &currGattDescriptor);
							}
						}

						// Activation des notifications
						if ((currGattChar->IsNotifiable) && (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x302))
						{
							// On initialise une variable qui portera le numéro du thingy tracé.
							CallBackValueThingyUI[Index] = Index;
							// Et le type d'event à surveiller
							BTH_LE_GATT_EVENT_TYPE EventType = CharacteristicValueChangedEvent;												
							//
							HRESULT ret2 = AttachNotification(
											hLEDeviceThingyUI[Index], &currGattChar, 
											&EventParameterInUI[Index], EventType, &UICallBackThingy[Index], 
											&CallBackValueThingyUI[Index], &EventHandleThingyUI[Index]);							
						}
						// Libération de la memoire
						FreeBluetoothDescriptors(&pDescriptorBufferUI[Index]);
					}
				}
			}
		}
	}
	   
	void readCharacteristic(int Index)
	{
		HRESULT hr;
		PBTH_LE_GATT_CHARACTERISTIC currGattChar;
		PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer;
		USHORT charValueDataSize;

		for (int ii = 0; ii < CharacteristicBufferCountUI[Index]; ii++)
		{
			// 
			currGattChar = &pCharBufferUI[Index][ii];
			//
			if (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x301)
			{
				if (currGattChar->IsReadable) {
					hr = BluetoothGATTGetCharacteristicValue(
						hLEDeviceThingyUI[Index],
						currGattChar,
						0,
						NULL,
						&charValueDataSize,
						BLUETOOTH_GATT_FLAG_NONE);

					if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
						printf("BluetoothGATTGetCharacteristicValue - Buffer Size %d", hr);
					}

					pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)GlobalAlloc(GPTR, charValueDataSize);

					if (NULL == pCharValueBuffer) {
						printf("pCharValueBuffer out of memory\r\n");
					}
					else {
						RtlZeroMemory(pCharValueBuffer, charValueDataSize);
					}

					hr = BluetoothGATTGetCharacteristicValue(
						hLEDeviceThingyUI[Index],
						currGattChar,
						(ULONG)charValueDataSize,
						pCharValueBuffer,
						NULL,
						BLUETOOTH_GATT_FLAG_NONE);

					if (S_OK != hr) {
						printf("BluetoothGATTGetCharacteristicValue - Actual Data %d", hr);
					}
				}
			}
		}
	}


	void WriteCharacteristic(int Index,UCHAR ENABLE,UCHAR RED , UCHAR GREEN,UCHAR BLUE)
	{
		HRESULT hr;

		USHORT charValueDataSize;
		PBTH_LE_GATT_CHARACTERISTIC_VALUE pCharValueBuffer;
		PBTH_LE_GATT_CHARACTERISTIC currGattChar;

		UCHAR valueData[4] = { 0x01,0x00,0x00,0x00 };

		valueData[0] = ENABLE;
		valueData[1] = RED;
		valueData[2] = GREEN;
		valueData[3] = BLUE;
		
		for (int ii = 0; ii < CharacteristicBufferCountUI[Index]; ii++)
		{
			// 
			currGattChar = &pCharBufferUI[Index][ii];
			//

			if (currGattChar->CharacteristicUuid.Value.ShortUuid == 0x301)
			{
				// On commence par obtenir une sturcture

				if (currGattChar->IsReadable)
				{
					hr = BluetoothGATTGetCharacteristicValue(
						hLEDeviceThingyUI[Index],
						currGattChar,
						0,
						NULL,
						&charValueDataSize,
						BLUETOOTH_GATT_FLAG_NONE);

					if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) != hr) {
						printf("BluetoothGATTGetCharacteristicValue - Buffer Size %d", hr);
					}

					pCharValueBuffer = (PBTH_LE_GATT_CHARACTERISTIC_VALUE)GlobalAlloc(GPTR, charValueDataSize);

					if (NULL == pCharValueBuffer) {
						printf("pCharValueBuffer out of memory\r\n");
					}
					else {
						RtlZeroMemory(pCharValueBuffer, charValueDataSize);
					}

					hr = BluetoothGATTGetCharacteristicValue(
						hLEDeviceThingyUI[Index],
						currGattChar,
						(ULONG)charValueDataSize,
						pCharValueBuffer,
						NULL,
						BLUETOOTH_GATT_FLAG_NONE);

					if (S_OK != hr) {
						printf("BluetoothGATTGetCharacteristicValue - Actual Data %d", hr);
					}

					// Calls to BluetoothGATTSetCharacteristicValue
					pCharValueBuffer->DataSize = sizeof(valueData);
					memcpy(pCharValueBuffer->Data, (UCHAR*)valueData, sizeof(valueData));

					// Set the new characteristic value
					hr = BluetoothGATTSetCharacteristicValue(hLEDeviceThingyUI[Index],
						currGattChar,
						pCharValueBuffer,
						NULL,
						BLUETOOTH_GATT_FLAG_NONE);
					
				}
			}
		}
	}

	

	
	void CloseBluetoothThingyMotion(int Index)
	{
		BluetoothGATTUnregisterEvent(EventHandleThingyMotion[Index], BLUETOOTH_GATT_FLAG_NONE);
		CloseHandle(hLEDeviceThingyMotion[Index]);
		// Et on libere le tableau de caracteristic
		// Liberation de la memoire des charactéristiques
		FreeBluetoothCharacteristics(&pCharBufferMotion[Index]);
		// et le tableau des services
		// Liberation de l'allocation memoire des services
		FreeBluetoothServices(&pServiceBufferMotion[Index]);
	}
	
	void CloseBluetoothThingyUI(int Index)
	{
		// On desactive les notifications
		BluetoothGATTUnregisterEvent(EventHandleThingyUI[Index], BLUETOOTH_GATT_FLAG_NONE);
		CloseHandle(hLEDeviceThingyUI[Index]);
		// Et on libere le tableau de caracteristic
		// Liberation de la memoire des charactéristiques
		FreeBluetoothCharacteristics(&pCharBufferUI[Index]);
		// et le tableau des services
		// Liberation de l'allocation memoire des services
		FreeBluetoothServices(&pServiceBufferUI[Index]);
	}

/*
};
*/

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






int __cdecl  main(int argc, char *argv[], char *envp[])
{
	OpenBluetoothThingyUI(0);
	OpenBluetoothThingyMotion(0);

	
	//go into an inf loop that sleeps. you will ideally see notifications from the HR device
	while (1) 
	{
		Sleep(1000);
		
		printf("look for notification Motiuon");
		// On check si 1 seconde s'est écoulé dpeuis la derniere reception
		LONGLONG CompareTickCount64_1 = GetTickCount64();
		if ((CompareTickCount64_1 - TickCount64ThingyMotion[0]) > 500)
		{
			printf("Timeout 0");
			// nettoye la connexion
			CloseBluetoothThingyMotion(0);
			OpenBluetoothThingyMotion(0);
		}
		else
		{
			// On affiche les valeurs
			printf("Thingy 0: %f,%f,%f,%f\r\n", GetFloatWThingy(0), GetFloatXThingy(0), GetFloatYThingy(0), GetFloatZThingy(0));
			// On bascule l'état de la led
		}
		

	// Exemple pour le motion

	/*
	//
	//printf("look for notification");
	// On check si 1 seconde s'est écoulé dpeuis la derniere reception
		LONGLONG CompareTickCount64_2 = GetTickCount64();
		if ((CompareTickCount64_2 - TickCount64ThingyUI[1]) > 500)
		{
			printf("Timeout 1");
			// nettoye la connexion
			CloseBluetoothThingyMotion(1);
			OpenBluetoothThingyMotion(1);
		}
		else
		{
			// On affiche les valeurs
			printf("Thingy 1: %f,%f,%f,%f\r\n", 
				GetFloatWThingy(1), 
				GetFloatXThingy(1),
				GetFloatYThingy(1),
				GetFloatZThingy(1));
		}
	*/
	}

	
	CloseBluetoothThingyMotion(0);
	CloseBluetoothThingyUI(0);
	

	if (GetLastError() != NO_ERROR &&
		GetLastError() != ERROR_NO_MORE_ITEMS)
	{
		// Insert error handling here.
		return 1;
	}

	return 0;
}

//
// L'idéé pour ecrire l etat de la led.
// Lister les caracteristiques
// recuperer le hande qui va bien
// Ecrire la valeurs pour activer la led.
// le code ci dessous explique la methode.


// Ensuite activation de la led
// et pour finir lecture ecriture audio.
