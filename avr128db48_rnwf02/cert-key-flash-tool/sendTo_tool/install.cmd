@echo off
REM Description:        Microchip Firmware CERT-KEY Utility Installer
REM Date:               05/22/2023
REM REF:                C75117
REM AUTHOR:             C67043
REM VERSION:            1.0.0
REM
REM Revisions:
REM --------------------------------------------------------------------------
REM   06/06/2023    1.0.0   Initial release


SETLOCAL ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION
    set ver=1.0.0

    cls
    set TAB=    
    for %%i in (x,x,x,x,x) do echo.
    echo **************************************************************************
    echo ****               Microchip Technologies Corporation                 ****
    echo ****                   CERT-KEY Flash Utility INSTALLER               ****
    echo ****                       v%ver% by C67043                           ****
    echo ****                                                                  ****
    echo **************************************************************************

    rem Allow a parameter to set the copy directory and NOT install for execution
    set installDrv=C:
    set installDir=CertFlash
    set sendToDir=C:\Users\%USERNAME%\AppData\Roaming\Microsoft\Windows\SendTo
    set installFiles=.\file_transfer.exe,.\upload-cert-key.cmd,.\images\mc.ico

    rem Check if this is an INSTALL or UNINSTALL
    if /I "%1"=="-u" (
        goto UNINSTALL
    ) 

    REM Check if installed...if not install the app
    if not exist "%installDrv%\%installDir%" (
        md "%installDrv%\%installDir%" >nul
    )

    REM XCopy files to the installation directory
    for %%i in (%installFiles%) do (
        xcopy %%i %installDrv%\%installDir% /Y >nul
    )

    :SHORTCUTS
    set SCRIPT="%TEMP%\%RANDOM%-%RANDOM%-%RANDOM%-%RANDOM%.vbs"
    @REM set SCRIPT=CON
    REM *************************************************************
    REM ****      Create desktop shortcut(s) to CERT-KEY             *****
    REM *************************************************************
    REM Create a batch file shortcut for the script
        echo Set oWS = WScript.CreateObject("WScript.Shell") > %SCRIPT%
        echo sLinkFile = "%USERPROFILE%\Desktop\CERT-KEYFlash.lnk" >> %SCRIPT%
        echo Set oLink = oWS.CreateShortcut(sLinkFile) >> %SCRIPT%
        echo oLink.TargetPath = "%installDrv%\%installDir%\upload-cert-key.cmd" >> %SCRIPT%
        echo oLink.WorkingDirectory = "%installDrv%\%installDir%" >> %SCRIPT%
        echo oLink.Arguments = "" >> %SCRIPT%
        echo oLink.IconLocation = "%installDrv%\%installDir%\mc.ico" >> %SCRIPT%
        echo oLink.Description = "Microchip CERT-KEYFlash Utility" >> %SCRIPT%
        @REM oLink.HotKey
        @REM oLink.WindowStyle
        echo oLink.Save >> %SCRIPT%
        cscript /nologo %SCRIPT%
        del %SCRIPT%

    xcopy "%USERPROFILE%\Desktop\CERT-KEYFlash.lnk" %installDrv%\%installDir%\ /Y >nul
    xcopy  "%USERPROFILE%\Desktop\CERT-KEYFlash.lnk" "%sendToDir%\" /Y >nul

    if exist "%installDrv%\%installDir%" (
        echo %TAB%Installed '%installDrv%\%installDir%' application folder 
    ) else ( 
        echo %TAB%Installation of '%installDrv%\%installDir%' application folder FAILED)
    if exist "%USERPROFILE%\Desktop\CERT-KEYFlash.lnk" (
        echo %TAB%Installed Desktop '%installDir%' shortcut
    ) else ( 
        echo %TAB%Installation of Desktop '%installDir%' shortcut FAILED)
    if exist "%USERPROFILE%\Desktop\CERT-KEYFlash.lnk" (
        echo %TAB%Added '%installDir%' to SendTo menu
    ) else ( 
        echo %TAB%Adding '%installDir%' to SendTo menu FAILED)

    echo.
    echo Installation Complete.  
    echo.
    goto END   

    :UNINSTALL
        if exist "%installDrv%\%installDir%" ( rd "%installDrv%\%installDir%" /S /Q  & echo %TAB%Removed '%installDrv%\%installDir%' folder)
        if exist "%USERPROFILE%\Desktop\CERT-KEYFlash.lnk" ( del "%USERPROFILE%\Desktop\CERT-KEYFlash.lnk" & echo %TAB%Removed '%installDir%' shortcut from Desktop)
        if exist "%sendToDir%\CERT-KEYFlash.lnk" ( del "%sendToDir%\CERT-KEYFlash.lnk" & echo %TAB%Removed '%installDir%' from SendTo menu)
        echo.
        echo Un-install Complete.
        echo.
        goto END
    :END
    :ABORT

ENDLOCAL
echo.&echo Press any key to exit...
pause > nul
