/*
 * Copyright 1999, 2001 Silicon Graphics, Inc.
 * Copyright 2001 Ralf Baechle
 */
#ifndef _SPB_H_
#define _SPB_H_

#include <libarc/arc.h>

typedef struct {
	LONG(*Load) (char *, unsigned long, unsigned long *, unsigned long *);
	LONG(*Invoke) (unsigned long, unsigned long, unsigned long, char *[], char *[]);
	LONG(*Execute) (char *, unsigned long, char *[], char *[]);
	void(*Halt) (void);
	void(*PowerDown) (void);
	void(*Restart) (void);
	void(*Reboot) (void);
	void(*EnterInteractiveMode) (void);
	void *reserved1;
	COMPONENT *(*GetPeer) (COMPONENT *);
	COMPONENT *(*GetChild) (COMPONENT *);
	COMPONENT *(*GetParent) (COMPONENT *);
	LONG(*GetConfigurationData) (void *, COMPONENT *);
	COMPONENT *(*AddChild) (COMPONENT *, COMPONENT *, void *);
	LONG(*DeleteComponent) (COMPONENT *);
	COMPONENT *(*GetComponent) (char *);
	LONG(*SaveConfiguration) (void);
	SYSTEMID *(*GetSystemId) (void);
	MEMORYDESCRIPTOR *(*GetMemoryDescriptor) (MEMORYDESCRIPTOR *);
	void *reserved2;
	TIMEINFO *(*GetTime) (void);
	unsigned long(*GetRelativeTime) (void);
	LONG(*GetDirectoryEntry) (unsigned long, DIRECTORYENTRY *, unsigned long, unsigned long *);
	LONG(*Open) (char *, OPENMODE, unsigned long *);
	LONG(*Close) (unsigned long);
	LONG(*Read) (unsigned long, void *, unsigned long, unsigned long *);
	LONG(*GetReadStatus) (unsigned long);
	LONG(*Write) (unsigned long, void *, unsigned long, unsigned long *);
	LONG(*Seek) (unsigned long, LARGEINTEGER *, SEEKMODE);
	LONG(*Mount) (char *, MOUNTOPERATION);
	char *(*GetEnvironmentVariable) (char *);
	LONG(*SetEnvironmentVariable) (char *, char *);
	LONG(*GetFileInformation) (unsigned long, FILEINFORMATION *);
	LONG(*SetFileInformation) (unsigned long, unsigned long, unsigned long);
	void(*FlushAllCaches) (void);
	LONG(*TestUnicodeCharacter) (unsigned long, unsigned short);
	DISPLAY_STATUS *(*GetDisplayStatus) (unsigned long);
} FIRMWAREVECTOR;

#define RSTB_SIGNATURE	0x42545352

typedef struct _rstb {
	unsigned long RSTBSignature;
	unsigned long RSTBLength;
	unsigned short Version;
	unsigned short Revision;
	struct _rstb *NextRSTB;
	void *RestartAddress;
	unsigned long BootMasterID;
	unsigned long ProcessorID;
	unsigned long BootStatus;
	unsigned long Checksum;
	unsigned long SaveAreaLength;
	unsigned long SavedStateArea[1];
} RESTARTBLOCK;

typedef struct {
	unsigned long AdapterType;
	unsigned long AdapterVectorLength;
	void *AdapterVector;
} ADAPTER;

#define SPB_SIGNATURE	0x53435241

typedef struct {
	unsigned long SPBSignature;
	unsigned long SPBLength;
	unsigned short Version;
	unsigned short Revision;
	RESTARTBLOCK *RestartBlock;
	void *DebugBlock;
	void *GEVector;
	void *UTLBMissVector;
	unsigned long FirmwareVectorLength;
	FIRMWAREVECTOR *FirmwareVector;
	unsigned long PrivateVectorLength;
	void *PrivateVector;
	unsigned long AdapterCount;
	ADAPTER Adapters[1];
} SPB;

#define SystemParameterBlock	((SPB *) 0xA0001000UL)
#define FVector		(SystemParameterBlock->FirmwareVector)

#endif /* _SPB_H_ */
