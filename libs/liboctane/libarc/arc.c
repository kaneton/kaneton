/*
 * Copyright 1999 Silicon Graphics, Inc.
 */
#include "types.h"
#include "arc.h"
#include "spb.h"


LONG ArcLoad(CHAR * Path, ULONG TopAddr, ULONG * ExecAddr, ULONG * LowAddr)
{
	return FVector->Load(Path, TopAddr, ExecAddr, LowAddr);
}


LONG
ArcInvoke(ULONG ExecAddr,
	  ULONG StackAddr, ULONG Argc, CHAR * Argv[], CHAR * Envp[]
    )
{
	return FVector->Invoke(ExecAddr, StackAddr, Argc, Argv, Envp);
}


LONG ArcExecute(CHAR * Path, ULONG Argc, CHAR * Argv[], CHAR * Envp[])
{
	return FVector->Execute(Path, Argc, Argv, Envp);
}


VOID ArcHalt(VOID)
{
	FVector->Halt();
}


VOID ArcPowerDown(VOID)
{
	FVector->PowerDown();
}


VOID ArcRestart(VOID)
{
	FVector->Restart();
}


VOID ArcReboot(VOID)
{
	FVector->Reboot();
}


VOID ArcEnterInteractiveMode(VOID)
{
	FVector->EnterInteractiveMode();
}


COMPONENT *ArcGetPeer(COMPONENT * Current)
{
	return FVector->GetPeer(Current);
}


COMPONENT *ArcGetChild(COMPONENT * Current)
{
	return FVector->GetChild(Current);
}


COMPONENT *ArcGetParent(COMPONENT * Current)
{
	return FVector->GetParent(Current);
}


LONG
ArcGetConfigurationData(VOID * ConfigurationData, COMPONENT * Component)
{
	return FVector->GetConfigurationData(ConfigurationData, Component);
}


COMPONENT *ArcAddChild(COMPONENT * Current, COMPONENT * Template,
		       VOID * ConfigurationData)
{
	return FVector->AddChild(Current, Template, ConfigurationData);
}


LONG ArcDeleteComponent(COMPONENT * ComponentToDelete)
{
	return FVector->DeleteComponent(ComponentToDelete);
}


COMPONENT *ArcGetComponent(CHAR * Path)
{
	return FVector->GetComponent(Path);
}


LONG ArcSaveConfiguration(VOID)
{
	return FVector->SaveConfiguration();
}


SYSTEMID *ArcGetSystemId(VOID)
{
	return FVector->GetSystemId();
}


MEMORYDESCRIPTOR *ArcGetMemoryDescriptor(MEMORYDESCRIPTOR * Current)
{
	return FVector->GetMemoryDescriptor(Current);
}


TIMEINFO *ArcGetTime(VOID)
{
	return FVector->GetTime();
}


ULONG ArcGetRelativeTime(VOID)
{
	return FVector->GetRelativeTime();
}


LONG
ArcGetDirectoryEntry(ULONG FileID,
		     DIRECTORYENTRY * Buffer, ULONG N, ULONG * Count)
{
	return FVector->GetDirectoryEntry(FileID, Buffer, N, Count);
}


LONG ArcOpen(CHAR * Path, OPENMODE OpenMode, ULONG * FileID)
{
	return FVector->Open(Path, OpenMode, FileID);
}


LONG ArcClose(ULONG FileID)
{
	return FVector->Close(FileID);
}


LONG ArcRead(ULONG FileID, VOID * Buffer, ULONG N, ULONG * Count)
{
	return FVector->Read(FileID, Buffer, N, Count);
}


LONG ArcGetReadStatus(ULONG FileID)
{
	return FVector->GetReadStatus(FileID);
}


LONG ArcWrite(ULONG FileID, VOID * Buffer, ULONG N, ULONG * Count)
{
	return FVector->Write(FileID, Buffer, N, Count);
}


LONG ArcSeek(ULONG FileID, LARGEINTEGER * Position, SEEKMODE SeekMode)
{
	return FVector->Seek(FileID, Position, SeekMode);
}


LONG ArcMount(CHAR * Path, MOUNTOPERATION Operation)
{
	return FVector->Mount(Path, Operation);
}


CHAR *ArcGetEnvironmentVariable(CHAR * Name)
{
	return FVector->GetEnvironmentVariable(Name);
}


LONG ArcSetEnvironmentVariable(CHAR * Name, CHAR * Value)
{
	return FVector->SetEnvironmentVariable(Name, Value);
}


LONG ArcGetFileInformation(ULONG FileID, FILEINFORMATION * Information)
{
	return FVector->GetFileInformation(FileID, Information);
}


LONG
ArcSetFileInformation(ULONG FileID, ULONG AttributeFlags,
		      ULONG AttributeMask)
{
	return FVector->SetFileInformation(FileID, AttributeFlags,
					   AttributeMask);
}


VOID ArcFlushAllCaches(VOID)
{
	FVector->FlushAllCaches();
}


LONG ArcTestUnicodeCharacter(ULONG FileID, USHORT UnicodeCharacter)
{
	return FVector->TestUnicodeCharacter(FileID, UnicodeCharacter);
}


DISPLAY_STATUS *ArcGetDisplayStatus(ULONG FileID)
{
	return FVector->GetDisplayStatus(FileID);
}
