#include <ntddk.h>
#ifndef UZ_DEVICE_NAME
#define UZ_DEVICE_NAME L"\\Device\\ROOTKIT"
#endif
#ifndef UZ_DOSDRV_NAME
#define UZ_DOSDRV_NAME L"\\DosDevices\\ROOTKIT"
#endif

VOID DriverUnload( IN PDRIVER_OBJECT DriverObject ) 
{
	UNICODE_STRING DosDrvName = { 0 };

	RtlInitUnicodeString( & DosDrvName, UZ_DOSDRV_NAME );
	IoDeleteSymbolicLink( & DosDrvName );
	IoDeleteDevice( DriverObject->DeviceObject );
};

NTSTATUS DrvDummyFunc() 
{
       	return STATUS_NOT_IMPLEMENTED; 
};

NTSTATUS DriverEntry( IN OUT PDRIVER_OBJECT DriverObject,
		      IN PUNICODE_STRING RegistryPath )
{
	PDEVICE_OBJECT DeviceObject = NULL;
	NTSTATUS       LastStatus   = STATUS_SUCCESS;
	UNICODE_STRING DeviceName   = { 0 };
	UNICODE_STRING DosDrvName   = { 0 };

	RtlInitUnicodeString( & DeviceName, UZ_DEVICE_NAME );
	RtlInitUnicodeString( & DosDrvName, UZ_DOSDRV_NAME );
	LastStatus = IoCreateDevice( DriverObject,
				     0,
				     & DeviceName,
				     FILE_DEVICE_UNKNOWN,
				     FILE_DEVICE_SECURE_OPEN,
				     FALSE, & DeviceObject );

	if ( NT_SUCCESS(LastStatus) ) {
		for ( int i = 0 ; i < IRP_MJ_MAXIMUM_FUNCTION ; i++ )
			DriverObject->MajorFunction[i] = ( PVOID ) DrvDummyFunc ;

		DriverObject->DriverUnload = ( PVOID ) DriverUnload;
		DriverObject->Flags       &= ~DO_DEVICE_INITIALIZING;

		LastStatus = IoCreateSymbolicLink( & DosDrvName, & DeviceName );
	};
	return LastStatus;
};
