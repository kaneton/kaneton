/*
 * Copyright 1999, 2001 Silicon Graphics, Inc.
 * Copyright 2001 Ralf Baechle
 */
#ifndef _SPB_H_
#define _SPB_H_

#ifdef		O2
#include <o2.h>
#else
#include <octane.h>
#endif

#include <libarc/arc.h>

typedef struct {
	long(*Load) (char *, unsigned long, unsigned long *, unsigned long *);
	long(*Invoke) (unsigned long, unsigned long, unsigned long, char *[], char *[]);
	long(*Execute) (char *, unsigned long, char *[], char *[]);
	void(*Halt) (void);
	void(*PowerDown) (void);
	void(*Restart) (void);
	void(*Reboot) (void);
	void(*EnterInteractiveMode) (void);
	void *reserved1;
	COMPONENT *(*GetPeer) (COMPONENT *);
	COMPONENT *(*GetChild) (COMPONENT *);
	COMPONENT *(*GetParent) (COMPONENT *);
	long(*GetConfigurationData) (void *, COMPONENT *);
	COMPONENT *(*AddChild) (COMPONENT *, COMPONENT *, void *);
	long(*DeleteComponent) (COMPONENT *);
	COMPONENT *(*GetComponent) (char *);
	long(*SaveConfiguration) (void);
	SYSTEMID *(*GetSystemId) (void);
	MEMORYDESCRIPTOR *(*GetMemoryDescriptor) (MEMORYDESCRIPTOR *);
	void *reserved2;
	TIMEINFO *(*GetTime) (void);
	unsigned long(*GetRelativeTime) (void);
	long(*GetDirectoryEntry) (unsigned long, DIRECTORYENTRY *, unsigned long, unsigned long *);
	long(*Open) (char *, OPENMODE, unsigned long *);
	long(*Close) (unsigned long);
	long(*Read) (unsigned long, void *, unsigned long, unsigned long *);
	long(*GetReadStatus) (unsigned long);
	long(*Write) (unsigned long, void *, unsigned long, unsigned long *);
	long(*Seek) (unsigned long, LARGEINTEGER *, SEEKMODE);
	long(*Mount) (char *, MOUNTOPERATION);
	char *(*GetEnvironmentVariable) (char *);
	long(*SetEnvironmentVariable) (char *, char *);
	long(*GetFileInformation) (unsigned long, FILEINFORMATION *);
	long(*SetFileInformation) (unsigned long, unsigned long, unsigned long);
	void(*FlushAllCaches) (void);
	long(*TestUnicodeCharacter) (unsigned long, unsigned short);
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

#define SystemParameterBlock	((SPB *)SPB_ADDR32)

#ifdef C32
#define SystemParameterBlock	((SPB *)SPB_ADDR32)
#endif

#define FVector		(SystemParameterBlock->FirmwareVector)

#endif /* _SPB_H_ */
