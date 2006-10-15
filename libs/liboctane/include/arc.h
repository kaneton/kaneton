/*
 * Copyright 1999 Silicon Graphics, Inc.
 *           2001-03 Guido Guenther <agx@sixcpu.org>
 */
#ifndef _ARC_H_
#define _ARC_H_

#include "types.h"

#define ESUCCESS	0	/* No error                             */
#define E2BIG		1	/* Argument list too long               */
#define EACCES		2	/* Permission denied                    */
#define EAGAIN		3	/* Resource temporarily unavailable     */
#define EBADF		4	/* Bad file descriptor                  */
#define EBUSY		5	/* Resource busy                        */
#define EFAULT		6	/* Bad address                          */
#define EINVAL		7	/* Invalid argument                     */
#define EIO		8	/* Input/output error                   */
#define EISDIR		9	/* Is a directory                       */
#define EMFILE		10	/* Too many open files                  */
#define EMLINK		11	/* Too many links                       */
#define ENAMETOOLONG	12	/* Filename too long                    */
#define ENODEV		13	/* No such device                       */
#define ENOENT		14	/* No such file or directory            */
#define ENOEXEC		15	/* Execute format error                 */
#define ENOMEM		16	/* Not enough space                     */
#define ENOSPC		17	/* No space left on device              */
#define ENOTDIR		18	/* Not a directory                      */
#define ENOTTY		19	/* Inappropriate I/O control operation  */
#define ENXIO		20	/* Media not loaded                     */
#define EROFS		21	/* Read-only file system                */


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
	USHORT Version;
	USHORT Revision;
	ULONG Key;
	ULONG AffinityMask;
	ULONG ConfigurationDataSize;
	ULONG IdentifierLength;
	CHAR *Identifier;
} COMPONENT;

typedef struct {
	CHAR VendorId[8];
	UCHAR ProductId[8];
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
	ULONG BasePage;
	ULONG PageCount;
} MEMORYDESCRIPTOR;

typedef struct {
	USHORT Year;
	USHORT Month;
	USHORT Day;
	USHORT Hour;
	USHORT Minutes;
	USHORT Seconds;
	USHORT Milliseconds;
} TIMEINFO;

#define ARC_NAME_MAX	32

typedef struct {
	ULONG FileNameLength;
	ULONG FileAttribute;
	ULONG FileName[ARC_NAME_MAX];
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
	ULONG LowPart;
	*LONG HighPart;
#else /* !(__MIPSEL__) */
	LONG HighPart;
	ULONG LowPart;
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
	ULONG FileNameLength;
	UCHAR Attributes;
	CHAR Filename[ARC_NAME_MAX];
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
	USHORT CursorXPosition;
	USHORT CursorYPosition;
	USHORT CursorMaxXPosition;
	USHORT CursorMaxYPosition;
	UCHAR ForegroundColor;
	UCHAR BackgroundColor;
	UCHAR HighIntensity;
	UCHAR Underscored;
	UCHAR ReverseVideo;
} DISPLAY_STATUS;


#define ARC_STDIN	0
#define ARC_STDOUT	1

extern LONG ArcLoad(CHAR * Path, ULONG TopAddr, ULONG * ExecAddr,
		    ULONG * LowAddr);
extern LONG ArcInvoke(ULONG ExecAddr, ULONG StackAddr, ULONG Argc,
		      CHAR * Argv[], CHAR * Envp[]);
extern LONG ArcExecute(CHAR * Path, ULONG Argc, CHAR * Argv[],
		       CHAR * Envp[]);
extern VOID ArcHalt(VOID);
extern VOID ArcPowerDown(VOID);
extern VOID ArcRestart(VOID);
extern VOID ArcReboot(VOID);
extern VOID ArcEnterInteractiveMode(VOID);
extern COMPONENT *ArcGetPeer(COMPONENT * Current);
extern COMPONENT *ArcGetChild(COMPONENT * Current);
extern COMPONENT *ArcGetParent(COMPONENT * Current);
extern LONG ArcGetConfigurationData
    (VOID * ConfigurationData, COMPONENT * Component);
extern COMPONENT *ArcAddChild
    (COMPONENT * Current, COMPONENT * Template, VOID * ConfigurationData);
extern LONG ArcDeleteComponent(COMPONENT * ComponentToDelete);
extern COMPONENT *ArcGetComponent(CHAR * Path);
extern LONG ArcSaveConfiguration(VOID);
extern SYSTEMID *ArcGetSystemId(VOID);
extern MEMORYDESCRIPTOR *ArcGetMemoryDescriptor(MEMORYDESCRIPTOR *
						Current);
extern TIMEINFO *ArcGetTime(VOID);
extern ULONG ArcGetRelativeTime(VOID);
extern LONG ArcGetDirectoryEntry
    (ULONG FileID, DIRECTORYENTRY * Buffer, ULONG N, ULONG * Count);
extern LONG ArcOpen(CHAR * Path, OPENMODE OpenMode, ULONG * FileID);
extern LONG ArcClose(ULONG FileID);
extern LONG ArcRead(ULONG FileID, VOID * Buffer, ULONG N, ULONG * Count);
extern LONG ArcGetReadStatus(ULONG FileID);
extern LONG ArcWrite(ULONG FileID, VOID * Buffer, ULONG N, ULONG * Count);
extern LONG ArcSeek(ULONG FileID, LARGEINTEGER * Position,
		    SEEKMODE SeekMode);
extern LONG ArcMount(CHAR * Path, MOUNTOPERATION Operation);
extern CHAR *ArcGetEnvironmentVariable(CHAR * Name);
extern LONG ArcSetEnvironmentVariable(CHAR * Name, CHAR * Value);
extern LONG ArcGetFileInformation(ULONG FileID,
				  FILEINFORMATION * Information);
extern LONG ArcSetFileInformation(ULONG FileID, ULONG AttributeFlags,
				  ULONG AttributeMask);
extern VOID ArcFlushAllCaches(VOID);
extern LONG ArcTestUnicodeCharacter(ULONG FileID, USHORT UnicodeCharacter);
extern DISPLAY_STATUS *ArcGetDisplayStatus(ULONG FileID);

#endif				/* _ARC_H_ */
