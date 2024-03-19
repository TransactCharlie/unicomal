@echo off
rem ---- Load btrieve system, run database program, discard btrieve system
if not exist btrieve.exe goto error
if not exist butil.exe goto error
btrieve
if errorlevel 1 goto error
database
if errorlevel 1 goto error
butil -STOP
if errorlevel 1 goto error
goto end
:error
echo -
echo -	The Btrieve sample program can not run without a btrieve system.
echo -
echo -	You must be owner of a Btrieve package, or it might have
echo -	followed with your Novell network.
echo -
echo -	The files  BTRIEVE.EXE and BUTIL.EXE must be present in this
echo -	directory.
echo -
:end
