@echo off
REM © 2023 Microchip Technology Inc. and its subsidiaries
REM Subject to your compliance with these terms, you may use this Microchip software
REM and any derivatives exclusively with Microchip products. You are responsible for 
REM complying with third party license terms applicable to your use of third party 
REM software (including open source software) that may accompany this Microchip 
REM software.
REM Redistribution of this Microchip software in source or binary form is allowed and
REM must include the above terms of use and the following disclaimer with the 
REM distribution and accompanying materials.
REM SOFTWARE IS “AS IS.” NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY 
REM TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
REM MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
REM LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL LOSS, 
REM DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER 
REM CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
REM FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP’S TOTAL LIABILITY ON 
REM ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT EXCEED AMOUNT OF FEES, IF ANY, YOU 
REM PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

REM Description:        Microchip Drag & Drop Utility
REM Date:               05/23/2023
REM VERSION:            1.0.0
REM
REM Revisions:
REM --------------------------------------------------------------------------
REM   05/23/2023    1.0.0   Initial release

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

@REM C:\CertFlash\file_upload.py load %EXT% -f %FN% -p %FILE%
C:\CertFlash\file_upload.exe load %EXT% -f %FN% -p %FILE%

:ERROR
timeout 10