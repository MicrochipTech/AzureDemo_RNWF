# Test Device Interaction with the Cloud Application (AVR128DB48 Curiosity Nano)

1. Install the AVR128DB48 Curiosity Nano Evaluation Kit & RNWF02PC Add On Board (still powered on via USB Type-C cable) onto the Curiosity Nano Adapter base board as shown

    <img src=".//media/boards/AVR128DB48_CNANO_RNWF02_Top.png" width=500/>
    <img src=".//media/boards/AVR128DB48_RNWF02PC.png" width=500/>

    NOTE: Some USB ports may **not** be able to supply sufficient current to power both Curiosity Nano & RNWF02PC boards, which is why up to this point, the RNWF02PC Add On Board has been powered using the separate USB Type-C cable. At the end of this demo procedure, after verifying successful connection to Azure IoT Central, you can try removing the USB Type-C cable (and changing the jumper setting) and seeing if the system still works...

2. Use a micro-USB cable to connect the Curiosity Nano board directly to one of your PC's standard USB ports

    <img src=".//media/boards/PC_to_AVR128DB48.png" width=600/>

3. Launch a [Terminal Emulator](https://en.wikipedia.org/wiki/List_of_terminal_emulators) program of your choice and connect to the Curiosity Nano's Virtual COM Port at 115200 baud.

    - Windows: The correct one to select will most likely be the one that shows up as a PICkit4 On Board Virtual COM Port

        <img src=".//media/image17a.png" width=400 />

    - MacOS: The correct one to select will most likely be the one that contains "usbmodem" as part of the COM port name

        <img src=".//media/image17b.png" width=200 />

4. Launch the `MPLAB X` IDE (this tool should have been previously installed and most likely resides in the \Program Files\Microchip\ folder. Once the MPLAB X IDE has finished its initialization routines, you should notice the "Kit Window" that acknowledges an active connection to the Curiosity Nano Evaluation Kit.

    <img src=".//media/image18b.png"  width=400 />

5. Navigate to the main toolbar's `File` > `Open Project` operation to load the demo project folder (\*.X) located at `\AzureDemo_RNWF\RNWF02PC\firmware\avr128db48_rnwf02.X`

    <img src=".//media/image19a.png" width=200 />

   NOTE: If the `Configuration load error` message in red appears in the `Output` window, click on the `Resolve DFP for configuration: default` link

    <img src=".//media/image21a.png" width=500 />

6. Set the `avr128db48_rnwf02` project as the main (currently focused/active) project by right-clicking on it and selecting `Set as Main Project`

    <img src=".//media/image40.png" width=300 />

7. In the `Projects` window, open the `main.h` header file by double-clicking directly on the file name

    <img src=".//media/image22.png" width=200 />

8. In the `main.h` header file, set the necessary parameters corresponding to your Wi-Fi Access Point
    - `HOME_AP_SSID` (name of your Wi-Fi Access Point)
    - `HOME_AP_PASSPHRASE` (password for your Wi-Fi Access Point)

9. In the `Projects` window, open the `azure_pnp.h` header file by double-clicking directly on the file name

    <img src=".//media/image22a.png" width=200 />

10. In the `azure_pnp.h` header file, set the parameters corresponding to your IoT device

    - `AZURE_DEVICE_ID`: This should match the Common Name (CN) used in the device/client certificate (e.g. "rnwf02_device_01")
    - `AZURE_ID_SCOPE`: Look up the ID scope for your IoT Central application by using the left-hand navigation pane [Security > Permissions > Device connection groups]

        <img src="./media/ID_Scope_LookUp.png" alt="Script Configuration" width = 500/>

11. Verify the project properties are set correctly before building the project by executing the following steps:

    - right-click on the `avr128db48_rnwf02` project
    - select `Properties`
    - select `AVR128DB48 Curiosity Nano-SN` under `Connected Hardware Tool`
    - select the latest version for `CMSIS`
    - select the latest version for `AVR-Dx_DFP`
    - select the latest XC8 version for `Compiler Toolchain`

        <img src=".//media/image42.png" width=550 />

        **Note** If any changes were made in the project properties window, the `Apply` button should become enabled.  Make sure to hit the `Apply` button before hitting `OK`

12. Right-click on the active project and select `Clean`. Right-click the project again and select `Make and Program Device`. This operation will automatically build the project before attempting to program the target device.

13. After the `BUILD SUCCESSFUL` message appears in the Output window, the application HEX file will be programmed onto the development Board. Once programming has finished, the board will automatically reset and start running its application code.

14. Observe the debug messages that are being continuously output to the terminal window. On every firmware reset, the AVR128DB48 will query the RNWF02PC module for information on its Wi-Fi configuration and the names of all certificates and keys which are currently stored in the module's internal memory. Successful responses from the RNWF02PC module should look something like the following terminal window messages:

    <img src=".//media/RNWF_Responses_Good.png" width=900 />

    However, if there is a problem with the RNWF02PC module, it will not respond to the Host MCU's queries for information on Wi-Fi/certificates/keys, so there will be blank lines displayed under each query as shown in the following terminal window output:

    <img src=".//media/RNWF_Responses_Bad.png" width=250 />

    Whenever these communication issues are observed between the Host MCU and RNWF02PC module, try resetting the AVR128DB48 Host MCU by holding down the `SW0` user button for at least 5 seconds and then releasing the button. If the RNWF02PC module still fails to respond to the read commands, try cycling power to the RNWF02PC Add On Board before resetting the AVR128DB48 Curiosity Nano board.

15. Assuming the RNWF02PC module successfully responded to each of the queries for Wi-Fi configuration, certificate file names, and key file names, it may take 1-2 minutes to establish the connections with both the Device Provisioning Service (DPS) and the IoT Hub used by the IoT Central application. Upon successful registration with DPS and connection to IoT Hub, the terminal output messages should look something like the following:

    <img src=".//media/Connection_Messages.png" width=800 />

16. The connection to Azure IoT Central is successful if telemetry messages are being continuously output to the terminal window. If the program appears to stall during the connection process (e.g. minutes of inactivity in the terminal window), you can hold down the `SW0` user button on the AVR128DB48 Curiosity Nano board for at least 5 seconds and then release the button to trigger a software reset. It may also be necessary to cycle power to the RNWF02PC Add On Board before resetting the AVR128DB48 Curiosity Nano board.

    <img src=".//media/Terminal_Output.png" width=1000/>

17. Try pressing the `SW0` mechanical button on the Curiosity Nano board and see if the number of presses is being output to the terminal window.

18. Access your IoT Central application by signing into the [IoT Central Portal](https://apps.azureiotcentral.com), clicking on `My Apps` in the left-hand side navigation pane, and then clicking on the tile that is labeled with the name of your application.

19. Click [here](./DeviceTemplate_CreatingViews.md) and follow the procedure to create an additional "Properties" view that allows you to read and/or change any of the cloud-writable properties.

20. Using the left-hand navigation pane, click on `Devices` under **Connect**, and then click on your device name

    <img src=".//media/image89.png" width=1000 />

21. Confirm that telemetry messages are being continuously received from the device by clicking on the **Raw Data** tab. Note the date/time each message was received - you should be seeing messages being continuously received during the present day/time.

    <img src=".//media/image90.png" width=1000 />

22. Click on the **Properties** tab. Select one of the 3 options for the property `LED0` and click on the **Save** icon. Observe that the `LED0` on the AVR128DB48 Curiosity Nano Board behaves in the expected mode.

    <img src=".//media/boards/AVR128DB48_LED0.png" width=500 />

    Try changing the state of the `LED0` to switch between the Off, On, and Blinking modes. The Telemetry Reporting Rate can also be adjusted to change the period (in seconds) for sending the continuous telemetry reports.

23. Click on the **Commands** view. Type any text message in the "Message to echo" box and then click on the **Run** button. To see the response from the device, click on the **command history** link (located just underneath the **Run** button). You should see that the response was received "now" (i.e. within the last minute) and that the correct message was echoed from the device.

    <img src=".//media/image92.png" width=400 />
    <img src=".//media/image93.png" width=300 />

24. Click on the **Commands** view. Type `PT5S` in the "Delay before reboot operation" box and then click on the **Run** button. To see the response from the device, click on the **command history** link (located just underneath the **Run** button). You should see that the response was received "now" (i.e. within the last minute) and that a success message was received from the device.

    <img src=".//media/image94.png" width=400 />
    <img src=".//media/image95.png" width=300 />

    "PT5S" is an [ISO 8601](https://en.wikipedia.org/wiki/ISO_8601) period designator, which decodes to 5 seconds following the standard's definition.  This demo firmware only supports periods in seconds, but the standard covers durations of years, months, days, etc.

        'P'eriod 
        'T'ime 
         5
        'S'econds

    NOTE: You can access any of your IoT Central applications in the future by accessing the [IoT Central Portal](https://apps.azureiotcentral.com).

25. At this point, assuming everything is working as described, you can see if your USB hub can supply enough current to power both boards using just the single USB cable connected to the AVR128DB48 Curiosity Nano. Disconnect the USB Type-C cable from the RNWF02PC Add On Board. Move the jumper so that the 2 pins furthest away from the module are shorted.

    <img src=".//media/boards/RNWF02_Power_VCC.png" width=500/>

26. Reset the AVR128DB48 Curiosity Nano by disconnecting and reconnecting the USB cable, or by holding down the `SW0` user button for at least 5 seconds and then releasing the button. Hopefully the entire connection process repeats successfully as shown in the following example terminal output!

    <img src=".//media/Terminal_Output.png" width=1000/>
