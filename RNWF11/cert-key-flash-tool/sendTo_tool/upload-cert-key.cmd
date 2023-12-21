@REM Reference: C75117
@echo off
cls

rem Parse parameter 1 into a path, filename & extension
    set PTH=%~d1%~p1
    set FN=%~n1
    set EXT=%~x1
    set FILE=%1

for /F "tokens=1,2 delims=." %%a in ("%FN%%EXT%") do (
  set FN=%%a
  set EXT=%%b
)

  if %EXT%==crt (
       set EXT=cert
  )  else if %EXT%==pem (
       set EXT=cert
  )  else if %EXT%==key (
       set EXT=key
  ) else (
        echo Invalid file type. Supported file types are: .pem, .crt or .key only.
        goto ERROR
  )

@REM C:\CertFlash\file_transfer.py load %EXT% -f %FN% -p %FILE%
C:\CertFlash\file_transfer.exe load %EXT% -f %FN% -p %FILE%

:ERROR
timeout 10