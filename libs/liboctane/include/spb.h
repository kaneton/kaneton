/*
 * Copyright 1999, 2001 Silicon Graphics, Inc.
 * Copyright 2001 Ralf Baechle
 */
#ifndef _SPB_H_
#define _SPB_H_

#include "types.h"
#include "arc.h"

typedef struct {
	LONG(*Load) (CHAR *, ULONG, ULONG *, ULONG *);
	LONG(*Invoke) (ULONG, ULONG, ULONG, CHAR *[], CHAR *[]);
	LONG(*Execute) (CHAR *, ULONG, CHAR *[], CHAR *[]);
	VOID(*Halt) (VOID);
	VOID(*PowerDown) (VOID);
	VOID(*Restart) (VOID);
	VOID(*Reboot) (VOID);
	VOID(*EnterInteractiveMode) (VOID);
	VOID *reserved1;
	COMPONENT *(*GetPeer) (COMPONENT *);
	COMPONENT *(*GetChild) (COMPONENT *);
	COMPONENT *(*GetParent) (COMPONENT *);
	LONG(*GetConfigurationData) (VOID *, COMPONENT *);
	COMPONENT *(*AddChild) (COMPONENT *, COMPONENT *, VOID *);
	LONG(*DeleteComponent) (COMPONENT *);
	COMPONENT *(*GetComponent) (CHAR *);
	LONG(*SaveConfiguration) (VOID);
	SYSTEMID *(*GetSystemId) (VOID);
	MEMORYDESCRIPTOR *(*GetMemoryDescriptor) (MEMORYDESCRIPTOR *);
	VOID *reserved2;
	TIMEINFO *(*GetTime) (VOID);
	ULONG(*GetRelativeTime) (VOID);
	LONG(*GetDirectoryEntry) (ULONG, DIRECTORYENTRY *, ULONG, ULONG *);
	LONG(*Open) (CHAR *, OPENMODE, ULONG *);
	LONG(*Close) (ULONG);
	LONG(*Read) (ULONG, VOID *, ULONG, ULONG *);
	LONG(*GetReadStatus) (ULONG);
	LONG(*Write) (ULONG, VOID *, ULONG, ULONG *);
	LONG(*Seek) (ULONG, LARGEINTEGER *, SEEKMODE);
	LONG(*Mount) (CHAR *, MOUNTOPERATION);
	CHAR *(*GetEnvironmentVariable) (CHAR *);
	LONG(*SetEnvironmentVariable) (CHAR *, CHAR *);
	LONG(*GetFileInformation) (ULONG, FILEINFORMATION *);
	LONG(*SetFileInformation) (ULONG, ULONG, ULONG);
	VOID(*FlushAllCaches) (VOID);
	LONG(*TestUnicodeCharacter) (ULONG, USHORT);
	DISPLAY_STATUS *(*GetDisplayStatus) (ULONG);
} FIRMWAREVECTOR;

#define RSTB_SIGNATURE	0x42545352

typedef struct _rstb {
	ULONG RSTBSignature;
	ULONG RSTBLength;
	USHORT Version;
	USHORT Revision;
	struct _rstb *NextRSTB;
	VOID *RestartAddress;
	ULONG BootMasterID;
	ULONG ProcessorID;
	ULONG BootStatus;
	ULONG Checksum;
	ULONG SaveAreaLength;
	ULONG SavedStateArea[1];
} RESTARTBLOCK;

typedef struct {
	ULONG AdapterType;
	ULONG AdapterVectorLength;
	VOID *AdapterVector;
} ADAPTER;

#define SPB_SIGNATURE	0x53435241

typedef struct {
	ULONG SPBSignature;
	ULONG SPBLength;
	USHORT Version;
	USHORT Revision;
	RESTARTBLOCK *RestartBlock;
	VOID *DebugBlock;
	VOID *GEVector;
	VOID *UTLBMissVector;
	ULONG FirmwareVectorLength;
	FIRMWAREVECTOR *FirmwareVector;
	ULONG PrivateVectorLength;
	VOID *PrivateVector;
	ULONG AdapterCount;
	ADAPTER Adapters[1];
} SPB;

#define SystemParameterBlock	((SPB *) 0xA0001000UL) 
#define FVector		(SystemParameterBlock->FirmwareVector)

#endif /* _SPB_H_ */
