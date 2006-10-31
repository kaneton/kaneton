/*
 * Copyright 1999 Silicon Graphics, Inc.
 *           2001-03 Guido Guenther <agx@sixcpu.org>
 */
#ifndef _ARC_H_
#define _ARC_H_

#define ENAMETOOLONG	12



typedef enum {
	SystemClass = 0,
	ProcessorClass = 1,
	CacheClass = 2,
	AdapterClass = 3,
	ControllerClass = 4,
	PeripheralClass = 5,
	MemoryClass = 6
} COMPONENT_CLASS;

typedef enum {
	ARC = 0,
	CPU = 1,
	FPU = 2,
	PrimaryICache = 3,
	PrimaryDCache = 4,
	SecondaryICache = 5,
	SecondaryDCache = 6,
	SecondaryCache = 7,
	EISAAdapter = 8,
	TCAdapter = 9,
	SCSIAdapter = 10,
	DTIAdapter = 11,
	MultiFunctionAdapter = 12,
	DiskController = 13,
	TapeController = 14,
	CDROMController = 15,
	WORMController = 16,
	SerialController = 17,
	NetworkController = 18,
	DisplayController = 19,
	ParallelController = 20,
	PointerController = 21,
	KeyboardController = 22,
	AudioController = 23,
	OtherController = 24,
	DiskPeripheral = 25,
	FloppyDiskPeripheral = 26,
	TapePeripheral = 27,
	ModemPeripheral = 28,
	MonitorPeripheral = 29,
	PrinterPeripheral = 30,
	PointerPeripheral = 31,
	KeyboardPeripheral = 32,
	TerminalPeripheral = 33,
	OtherPeripheral = 34,
	LinePeripheral = 35,
	NetworkPeripheral = 36,
	MemoryUnit = 37
} COMPONENT_TYPE;

typedef enum {
	Failed = 1,
	ReadOnly = 2,
	Removable = 4,
	ConsoleIn = 8,
	ConsoleOut = 16,
	Input = 32,
	Output = 64
} COMPONENT_FLAGS;

typedef struct {
	COMPONENT_CLASS Class;
	COMPONENT_TYPE Type;
	COMPONENT_FLAGS Flags;
	unsigned short Version;
	unsigned short Revision;
	unsigned long Key;
	unsigned long AffinityMask;
	unsigned long ConfigurationDataSize;
	unsigned long IdentifierLength;
	char *Identifier;
} COMPONENT;

typedef struct {
	char VendorId[8];
	unsigned char ProductId[8];
} SYSTEMID;

/* This is ARCS not ARC */
typedef enum {
	ExceptionBlock,
	SystemParameterBlock,
	FreeContiguous,
	FreeMemory,
	BadMemory,
	LoadedProgram,
	FirmwareTemporary,
	FirmwarePermanent,
} MEMORYTYPE;

typedef struct {
	MEMORYTYPE Type;
	unsigned long BasePage;
	unsigned long PageCount;
} MEMORYDESCRIPTOR;

typedef struct {
	unsigned short Year;
	unsigned short Month;
	unsigned short Day;
	unsigned short Hour;
	unsigned short Minutes;
	unsigned short Seconds;
	unsigned short Milliseconds;
} TIMEINFO;

#define ARC_NAME_MAX	32

typedef struct {
	unsigned long FileNameLength;
	unsigned long FileAttribute;
	unsigned long FileName[ARC_NAME_MAX];
} DIRECTORYENTRY;

typedef enum {
	OpenReadOnly,
	OpenWriteOnly,
	OpenReadWrite,
	CreateWriteOnly,
	CreateReadWrite,
	SupersedeWriteOnly,
	SupersedeReadWrite,
	OpenDirectory,
	CreateDirectory
} OPENMODE;

typedef struct {
#ifdef __MIPSEL__
	unsigned long LowPart;
	*long HighPart;
#else /* !(__MIPSEL__) */
	long HighPart;
	unsigned long LowPart;
#endif
} LARGEINTEGER;

typedef enum {
	SeekAbsolute,
	SeekRelative
} SEEKMODE;

typedef enum {
	LoadMedia,
	UnloadMedia
} MOUNTOPERATION;

typedef struct {
	LARGEINTEGER StartingAddress;
	LARGEINTEGER EndingAddress;
	LARGEINTEGER CurrentAddress;
	COMPONENT_TYPE Type;
	unsigned long FileNameLength;
	unsigned char Attributes;
	char Filename[ARC_NAME_MAX];
} FILEINFORMATION;

typedef enum {
	ReadOnlyFile = 1,
	HiddenFile = 2,
	SystemFile = 4,
	ArchiveFile = 8,
	DirectoryFile = 16,
	DeleteFile = 32
} FILEATTRIBUTES;

typedef struct {
	unsigned short CursorXPosition;
	unsigned short CursorYPosition;
	unsigned short CursorMaxXPosition;
	unsigned short CursorMaxYPosition;
	unsigned char ForegroundColor;
	unsigned char BackgroundColor;
	unsigned char HighIntensity;
	unsigned char Underscored;
	unsigned char ReverseVideo;
} DISPLAY_STATUS;


#define ARC_STDIN	0
#define ARC_STDOUT	1

extern long ArcLoad(char * Path, unsigned long TopAddr, unsigned long * ExecAddr,
		    unsigned long * LowAddr);
extern long ArcInvoke(unsigned long ExecAddr, unsigned long StackAddr, unsigned long Argc,
		      char * Argv[], char * Envp[]);
extern long ArcExecute(char * Path, unsigned long Argc, char * Argv[],
		       char * Envp[]);
extern void ArcHalt(void);
extern void ArcPowerDown(void);
extern void ArcRestart(void);
extern void ArcReboot(void);
extern void ArcEnterInteractiveMode(void);
extern COMPONENT *ArcGetPeer(COMPONENT * Current);
extern COMPONENT *ArcGetChild(COMPONENT * Current);
extern COMPONENT *ArcGetParent(COMPONENT * Current);
extern long ArcGetConfigurationData
    (void * ConfigurationData, COMPONENT * Component);
extern COMPONENT *ArcAddChild
    (COMPONENT * Current, COMPONENT * Template, void * ConfigurationData);
extern long ArcDeleteComponent(COMPONENT * ComponentToDelete);
extern COMPONENT *ArcGetComponent(char * Path);
extern long ArcSaveConfiguration(void);
extern SYSTEMID *ArcGetSystemId(void);
extern MEMORYDESCRIPTOR *ArcGetMemoryDescriptor(MEMORYDESCRIPTOR *
						Current);
extern TIMEINFO *ArcGetTime(void);
extern unsigned long ArcGetRelativeTime(void);
extern long ArcGetDirectoryEntry
    (unsigned long FileID, DIRECTORYENTRY * Buffer, unsigned long N, unsigned long * Count);
extern long ArcOpen(char * Path, OPENMODE OpenMode, unsigned long * FileID);
extern long ArcClose(unsigned long FileID);
extern long ArcRead(unsigned long FileID, void * Buffer, unsigned long N, unsigned long * Count);
extern long ArcGetReadStatus(unsigned long FileID);
extern long ArcWrite(unsigned long FileID, void * Buffer, unsigned long N, unsigned long * Count);
extern long ArcSeek(unsigned long FileID, LARGEINTEGER * Position,
		    SEEKMODE SeekMode);
extern long ArcMount(char * Path, MOUNTOPERATION Operation);
extern char *ArcGetEnvironmentVariable(char * Name);
extern long ArcSetEnvironmentVariable(char * Name, char * Value);
extern long ArcGetFileInformation(unsigned long FileID,
				  FILEINFORMATION * Information);
extern long ArcSetFileInformation(unsigned long FileID, unsigned long AttributeFlags,
				  unsigned long AttributeMask);
extern void ArcFlushAllCaches(void);
extern long ArcTestUnicodeCharacter(unsigned long FileID, unsigned short UnicodeCharacter);
extern DISPLAY_STATUS *ArcGetDisplayStatus(unsigned long FileID);


int write(char c);

#endif				/* _ARC_H_ */
