/*
 * Copyright 1999 Silicon Graphics, Inc.
 */

#include <libarc/arc.h>
#include <libarc/spb.h>

typedef unsigned long FILE;
extern FILE *stdin;
extern FILE *stdout;
static FILE arc_stdin = ARC_STDIN;
FILE *stdin = &arc_stdin;

static FILE arc_stdout = ARC_STDOUT;
FILE *stdout = &arc_stdout;

int		write(char c)
{
  unsigned long	count = 0;

  return (int) ArcWrite(*stdout, &c, 1, &count);
}

long ArcLoad(char * Path, unsigned long TopAddr, unsigned long * ExecAddr, unsigned long * LowAddr)
{
	return FVector->Load(Path, TopAddr, ExecAddr, LowAddr);
}


long
ArcInvoke(unsigned long ExecAddr,
	  unsigned long StackAddr, unsigned long Argc, char * Argv[], char * Envp[]
    )
{
	return FVector->Invoke(ExecAddr, StackAddr, Argc, Argv, Envp);
}


long ArcExecute(char * Path, unsigned long Argc, char * Argv[], char * Envp[])
{
	return FVector->Execute(Path, Argc, Argv, Envp);
}


void ArcHalt(void)
{
	FVector->Halt();
}


void ArcPowerDown(void)
{
	FVector->PowerDown();
}


void ArcRestart(void)
{
	FVector->Restart();
}


void ArcReboot(void)
{
	FVector->Reboot();
}


void ArcEnterInteractiveMode(void)
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


long
ArcGetConfigurationData(void * ConfigurationData, COMPONENT * Component)
{
	return FVector->GetConfigurationData(ConfigurationData, Component);
}


COMPONENT *ArcAddChild(COMPONENT * Current, COMPONENT * Template,
		       void * ConfigurationData)
{
	return FVector->AddChild(Current, Template, ConfigurationData);
}


long ArcDeleteComponent(COMPONENT * ComponentToDelete)
{
	return FVector->DeleteComponent(ComponentToDelete);
}


COMPONENT *ArcGetComponent(char * Path)
{
	return FVector->GetComponent(Path);
}


long ArcSaveConfiguration(void)
{
	return FVector->SaveConfiguration();
}


SYSTEMID *ArcGetSystemId(void)
{
	return FVector->GetSystemId();
}


MEMORYDESCRIPTOR *ArcGetMemoryDescriptor(MEMORYDESCRIPTOR * Current)
{
	return FVector->GetMemoryDescriptor(Current);
}


TIMEINFO *ArcGetTime(void)
{
	return FVector->GetTime();
}


unsigned long ArcGetRelativeTime(void)
{
	return FVector->GetRelativeTime();
}


long
ArcGetDirectoryEntry(unsigned long FileID,
		     DIRECTORYENTRY * Buffer, unsigned long N, unsigned long * Count)
{
	return FVector->GetDirectoryEntry(FileID, Buffer, N, Count);
}


long ArcOpen(char * Path, OPENMODE OpenMode, unsigned long * FileID)
{
	return FVector->Open(Path, OpenMode, FileID);
}


long ArcClose(unsigned long FileID)
{
	return FVector->Close(FileID);
}


long ArcRead(unsigned long FileID, void * Buffer, unsigned long N, unsigned long * Count)
{
	return FVector->Read(FileID, Buffer, N, Count);
}


long ArcGetReadStatus(unsigned long FileID)
{
	return FVector->GetReadStatus(FileID);
}


long ArcWrite(unsigned long FileID, void * Buffer, unsigned long N, unsigned long * Count)
{
	return FVector->Write(FileID, Buffer, N, Count);
}


long ArcSeek(unsigned long FileID, LARGEINTEGER * Position, SEEKMODE SeekMode)
{
	return FVector->Seek(FileID, Position, SeekMode);
}


long ArcMount(char * Path, MOUNTOPERATION Operation)
{
	return FVector->Mount(Path, Operation);
}


char *ArcGetEnvironmentVariable(char * Name)
{
	return FVector->GetEnvironmentVariable(Name);
}


long ArcSetEnvironmentVariable(char * Name, char * Value)
{
	return FVector->SetEnvironmentVariable(Name, Value);
}


long ArcGetFileInformation(unsigned long FileID, FILEINFORMATION * Information)
{
	return FVector->GetFileInformation(FileID, Information);
}


long
ArcSetFileInformation(unsigned long FileID, unsigned long AttributeFlags,
		      unsigned long AttributeMask)
{
	return FVector->SetFileInformation(FileID, AttributeFlags,
					   AttributeMask);
}


void ArcFlushAllCaches(void)
{
	FVector->FlushAllCaches();
}


long ArcTestUnicodeCharacter(unsigned long FileID, unsigned short UnicodeCharacter)
{
	return FVector->TestUnicodeCharacter(FileID, UnicodeCharacter);
}


DISPLAY_STATUS *ArcGetDisplayStatus(unsigned long FileID)
{
	return FVector->GetDisplayStatus(FileID);
}
