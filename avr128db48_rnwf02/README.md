# Provisioning the Microchip AVR128DB48 Curiosity Nano Evaluation Kit for Microsoft Azure IoT Services

## Introduction

This document describes how to connect the AVR128DB48 Curiosity Nano Evaluation Kit to a cloud application running on Microsoft's Azure IoT Central platform. Secure connections are made possible by using Certificate Authority signed X.509 certificate authentication between the Azure server and client (a.k.a. "device"). Wireless connectivity to the cloud is made possible by connecting Microchip's RNWF02PC module to one of the AVR128DB48's standard UART ports to serve an easy-to-use, serial-to-cloud bridge.

## Hardware Requirements

* [USB-to-UART Serial Adapter/Bridge/Converter](https://www.newark.com/c/cable-wire-cable-assemblies/cable-assemblies/usb-adapter-cables?conversion-type=usb-to-uart-converter)

    <img src=".//media/MFG_TTL-232R-RPI.jpg" width=150/>

* [AVR128DB48 Curiosity Nano Evaluation Kit](https://www.microchip.com/en-us/development-tool/ev35l43a)

    <img src=".//media/avr128db48_cnano.png" width=500/>

* [Curiosity Nano Base for Click Boards](https://www.microchip.com/en-us/development-tool/ac164162)

    <img src=".//media/cnano_base_click.png" width=300/>

* [RNWF02PC Add On Board]()

    <img src=".//media/RNWF02PC_Add_On_Board.png" width=300/>

## Hardware Preparation

1. Confirm that there is a jumper which is shorting the two pins closest to the RNWF02PC module

    <img src=".//media/RNWF02_Power_USB.png" width=500/>

2. Provide power to the RNWF02PC Add On Board by connecting a USB Type-C cable to a +5V DC power source (e.g. USB port or wall adapter)

    <img src=".//media/PC_to_RNWF02.png" width=500/>

3. Connect the [USB-to-UART converter](https://www.newark.com/c/cable-wire-cable-assemblies/cable-assemblies/usb-adapter-cables?conversion-type=usb-to-uart-converter) between the PC and the TX/RX/GND pins of the mikroBUS™ header on the RNWF02PC Add On Board:

    <img src=".//media/USB-UART_mikroBUS.png" width=500/>

## Software Prerequisites & Installation

1. [Git](https://git-scm.com/)
2. Any [Terminal Emulator](https://en.wikipedia.org/wiki/List_of_terminal_emulators) program of your choice
3. Microchip `MPLAB X` tool chain for embedded code development on 8-bit architecture MCU platforms (IDE & compiler)

    - [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)

    - [MPLAB XC8 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers#tabs)

        NOTE: This demonstration project was last tested successfully with XC8 v2.41, and in general should work with later versions of the compiler as they become available. If you encounter issues building the project with a newer version of the compiler, it is recommended to download the last known working version from the [MPLAB Development Ecosystem Downloads Archive](https://www.microchip.com/en-us/tools-resources/archives/mplab-ecosystem) (to fall back to the version Microchip successfully tested prior to release). 


## Create Chain of Trust and Cloud Application

### Step 1 - Clone this Repository

Create a clone of this repository using the [Git](https://git-scm.com) tool in a command line window

```bash
git clone https://github.com/MicrochipTech/AzureDemo_RNWF
```

As an alternative, scroll up towards the top of this page, click on the **Code** button, and download a ZIP file of this repository

<img src=".//media/Download_ZIP.png" width=400/>

### Step 2 - Generate the Chain of Trust (X.509 Certificates & Keys)

For secure connections, a chain of trust (which includes certificates & keys for the root, signer, and client) needs to be generated and programmed into the RNWF02PC module.

The client certificate file will be needed when we create the device in Azure IoT Central using the individual enrollment method. Another option is to use the group enrollment method which requires uploading the signer certificate file (or in some cases, could be the root certificate if the client certificate is chained directly to it) to the Azure IoT Central application, so that any device which presents a leaf certificate that was derived from the signer (or root) certificate will automatically be granted access to registration.

NOTE: For evaluation purposes, an example chain of trust has been provided in this [sub-folder](./cert-chain-gen-tool/). If you are not interested in going through the detailed steps of creating your own chain of trust at this time, feel free to jump directly to Step 2.2 below to use the provided set of example files.

#### 2.1 Generate the files for your certificates & keys

Click [here](./cert-chain-gen-scripts/) and follow the instructions to use Git Bash shell scripts to generate your chain of trust. During execution of the scripts, you will be asked to provide names for the Root CA, Signer (Subordinate/Intermediate) CA, and the Common Name (CN) for the device. The Common Name for the device will be used as the registration ID when connecting to the Azure IoT Central's Device Provisioning Service (DPS).

#### 2.2 Upload the Device's Client Certificate & Private Key into the RNWF02PC Module

* A PC-based utility has been provided to help you easily program the device's certificate & key files into the RNWF02PC module. If you are using the pre-existing default chain of trust that has been provided in this example, you will just need to program the `myDevice_cert.pem` & `myDevice_key.pem` files that are located in this [sub-folder](./cert-chain-gen-tool/). Note that the Common Name (CN) for the provided device certificate is "MyDevice" - which means the Device ID will correspond to this Common Name (CN) for registration/identification purposes in Azure.

    <img src=".//media/MyDevice_Files.png" width=600/>

* Open a `Command Prompt` window and use the command line to navigate to the `AzureDemo_RNWF\avr128db48_rnwf02\cert-key-flash-tool\sendTo_tool` directory. Execute the following command to install the `CERT-KEY` flash update tool:

        .\install.cmd

* Using a `File Explorer` window, navigate to the `AzureDemo_RNWF\avr128db48_rnwf02\cert-chain-gen-tool` folder

* Locate the `myDevice_cert.pem` file. Right-click on the file and select `Send to` > `CERT-KEYFlash`

    <img src=".//media/CERT-KEY_cert.png" width=600/>

* Locate the `myDevice_key.pem` file. Right-click on the file and select `Send to` > `CERT-KEYFlash`

    <img src=".//media/CERT-KEY_key.png" width=600/>

* Launch a [Terminal Emulator](https://en.wikipedia.org/wiki/List_of_terminal_emulators) program of your choice and connect to the USB-to-UART converter's Virtual COM Port at 230400 baud. Make sure that your terminal settings for Transmit equates to "CR+LF", and 'Local Echo' is disabled. Once the serial connection has been made, hit the [RETURN] key once or twice to see the command prompt generated by the RNWF02PC module. The module is now ready to accept AT commands.

* Issue the following command in the terminal to see a list of all certificates currently stored in the RNWF02PC module, and then confirm that "DigiCertGlobalRootG2" appears in the list:

    ```bash
    AT+FS=2,1
    ```

* Confirm that the device certificate (that was uploaded earlier) appears in the list (e.g. "myDevice_cert"). Issue the following command to configure the module to use `myDevice_cert` for the device certificate during the Transport Layer Security (TLS) authentication phase:

    ```bash
    AT+TLSC=1,2,"myDevice_cert"
    ```

* Issue the following command in the terminal to see a list of all keys currently stored in the RNWF02PC module:

    ```bash
    AT+FS=2,2
    ```

* Confirm that the device private key (that was uploaded earlier) appears in the list (e.g. "myDevice_key"). Issue the following command to configure the module to use `myDevice_key` for the device key during the Transport Layer Security (TLS) authentication phase:

    ```bash
    AT+TLSC=1,3,"myDevice_key"
    ```

* Issue the following commands to configure the module to use the correct root CA certificate and endpoint(s) required to authenticate the servers:

    ```bash
    AT+TLSC=1,1,"DigiCertGlobalRootG2"
    AT+TLSC=1,5,"*.azure-devices.net"
    ```

* To confirm that all the correct certificate/key files are set for use during the TLS authentication phase, issue the following command:

    ```bash
    AT+TLSC=1
    ```
* The response from the RNWF02PC module should look like the following (meaning the required certificates & key have been correctly selected for TLS authentication purposes):

   <img src=".//media/cert-key_correct.png" width=200/>

* If needed, the following commands can be issued to delete certificates and/or keys:

    ```bash
    AT+FS=3,1,"<CERTIFICATE_FILE_NAME>"
    AT+FS=3,2,"<KEY_FILE_NAME>"
    ```

* Upon successful completion of uploading the device certificate & key files to the RNWF02PC module (and configuring which ones the module should use during the TLS authentication phase), the USB-to-UART converter can now be disconnected from the RNWF02PC Add On Board (and close the serial connection for the Terminal Emulator window as well). Keep the RNWF02PC Add On Board powered on via the USB Type-C cable.

### Step 3 - Create an Azure IoT Central Application

#### 3.1 Create an Azure Account and Subscription

Microsoft has excellent instructions to create an new Azure account and subscription.  Please create an account and subscription before continuing. Review our overview of the [Azure Account/Subscription creation process](./CreateAzureAccountAndSubscription.md) as required.

#### 3.2 Create an Azure IoT Central Application

Click [here](CreateAnIoTCentralApplication.md) and follow the procedure to create an IoT Central application to be used with this demonstration.

NOTE: You can access any of your IoT Central applications in the future by accessing the [IoT Central Portal](https://apps.azureiotcentral.com).

### Step 4 - Enroll Your Device in the Azure IoT Central Application

- Access your IoT Central application by signing into the [IoT Central Portal](https://apps.azureiotcentral.com), clicking on `My Apps` in the left-hand side navigation pane, and then clicking on the tile that is labeled with the name of your application.

    <img src="./media/IOTC_MyApps.png" alt="Script Configuration" width = 250/>

- Click on one of the following procedures to pre-register your device with the IoT Central application (creating an enrollment group is strongly recommended as a more practical solution vs. registering each device individually):

    - [Create an Enrollment Group](./IoT_Central_Group_Enrollment.md)
    - [Create an Individual Enrollment](./IoT_Central_Individual_Enrollment.md)

**For Your Education**: [Group enrollment](https://learn.microsoft.com/en-us/azure/iot-dps/concepts-service#enrollment-group) allows you to create a group of allowable devices which each have a leaf certificate derived from a common signer or root certificate so that devices do not need to be pre-enrolled on an individual basis. Enrollment groups are used to enroll multiple related devices; [Individual enrollment](https://learn.microsoft.com/en-us/azure/iot-dps/concepts-service#individual-enrollment) is used to enroll a single device. Feel free to review both methods and be sure to complete the procedure for your preferred method before proceeding with the next step.

## Test Device Interaction with the Cloud Application

1. Install the AVR128DB48 Evaluation Kit & RNWF02PC Add On Board (still powered on via USB Type-C cable) onto the Curiosity Nano Adapter base board as shown

    <img src=".//media/AVR128DB48_RNWF02PC.png" width=500/>

    NOTE: Some USB hubs may not be able to supply enough current to power both AVR128DB48 & RNWF02PC boards, which is why up to this point, the RNWF02PC Add On Board has been powered on via the separate USB Type-C cable. At the end of this demo procedure, after verifying successful connection to Azure IoT Central, you can try removing the USB Type-C cable (and changing the jumper setting) and seeing if the system still works...

2. Use a micro-USB cable to connect the AVR128DB48 board directly to one of your PC's standard USB ports

    <img src=".//media/PC_to_AVR128DB48.png" width=500/>

3. Launch a [Terminal Emulator](https://en.wikipedia.org/wiki/List_of_terminal_emulators) program of your choice and connect to the AVR128DB48 Curiosity Nano's Virtual COM Port at 115200 baud.

    - Windows users: The correct one to select will most likely be the one that shows up as a Virtual COM Port

        <img src=".//media/image17a.png" width=400 />

    - MacOS users: The correct one to select will most likely be the one that contains "usbmodem" as part of the descriptor

        <img src=".//media/image17b.png" width=200 />

4. Launch the `MPLAB X` IDE (this tool should have been previously installed and most likely resides in the \Program Files\Microchip\ folder. Once the MPLAB X IDE has finished its initialization routines, you should notice the "Kit Window" that acknowledges an active connection to the AVR128DB48 Curiosity Nano Evaluation Kit

    <img src=".//media/image18b.png"  />

5. Navigate to the main toolbar's `File` > `Open Project` operation to load the demo project folder (\*.X) located at `\AzureDemo_RNWF\avr128db48_rnwf02\avr128db48_rnwf02.X`

    <img src=".//media/image19a.png" width=200 />
    <img src=".//media/image19b.png" width=400 />

   NOTE: If the `Configuration load error` message in red appears in the `Output` window, click on the `Resolve DFP for configuration: default` link

    <img src=".//media/image21a.png" width=500 />

6. Set the `avr128db48_rnwf02` project as the main (currently focused/active) project by right-clicking on it and selecting `Set as Main Project`

    <img src=".//media/image40.png" width=300 />

7. In the `Projects` window, open the `main.c` source file by double-clicking directly on the file name

    <img src=".//media/image41a.png" width=200 />

8. In the `main.c` source file, set the device certificate & key file names as they are listed in the RNWF02PC module (i.e. the same names of the PEM files which were programmed into the module previously)

    <img src=".//media/image41b.png" width=600 />

9. In the `Projects` window, open the `main.h` header file by double-clicking directly on the file name

    <img src=".//media/image22.png" width=200 />

10. In the `main.h` header file, set the necessary parameters corresponding to your Wi-Fi Access Point
    - `HOME_AP_SSID` (name of your Wi-Fi Access Point)
    - `HOME_AP_PASSPHRASE` (password for your Wi-Fi Access Point)

11. In the `Projects` window, open the `azure_pnp.h` header file by double-clicking directly on the file name

    <img src=".//media/image22a.png" width=200 />

12. In the `azure_pnp.h` header file, set the parameters corresponding to your IoT device

    - `AZURE_DEVICE_ID` (confirm this matches the Common Name (CN) used in the device/client certificate)
    - `AZURE_ID_SCOPE` (look up the ID scope for your IoT Central application by using the left-hand navigation pane [Security -> Permissions -> Device connection groups])

        <img src="./media/ID_Scope_LookUp.png" alt="Script Configuration" width = 500/>

13. Verify the project properties are set correctly before building the project by executing the following steps:

    - right-click on the `avr128db48_rnwf02` project
    - select `Properties`
    - under `Connected Hardware Tool`, select `AVR128DB48 Curiosity Nano-SN`
    - select the latest version for `CMSIS`
    - select the latest version for `AVR-Dx_DFP`
    - select the latest XC8 version for `Compiler Toolchain`

        <img src=".//media/image42.png" width=550 />

        **Note** If any changes were made in the project properties window, the `Apply` button should become enabled.  Make sure to hit the `Apply` button before hitting `OK`

14. Right-click on the active project and select `Clean`. Right-click the project again and select `Make and Program Device`. This operation will automatically build the project before attempting to program the target device.

15. After the `BUILD SUCCESSFUL` message appears in the Output window, the application HEX file will be programmed onto the development Board. Once programming has finished, the board will automatically reset and start running its application code.

16. Observe the debug messages that are being continuously output to the terminal window. The connection to Azure IoT Central is successful if telemetry messages are being continuously output to the terminal window. If the program appears to stall, you can hold down the `SW0` button on the AVR128DB48 Curiosity Nano board for approximately 4 seconds and then release the button to trigger a software reset. It may also be necessary to cycle power to the RNWF02PC Add On Board before resetting the AVR128DB48 Curiosity Nano board.

    <img src=".//media/Terminal_Output.png" width=700/>

17. Access your IoT Central application by signing into the [IoT Central Portal](https://apps.azureiotcentral.com), clicking on `My Apps` in the left-hand side navigation pane, and then clicking on the tile that is labeled with the name of your application.

18. Click [here](./DeviceTemplate_CreatingViews.md) and follow the procedure to create an additional "Properties" view that allows you to read and/or change any of the cloud-writable properties.

19. Using the left-hand navigation pane, click on `Devices` under **Connect**, and then click on your device name

    <img src=".//media/image89.png" width=800 />

20. Confirm that telemetry messages are being continuously received from the device by clicking on the **Raw Data** tab. Note the date/time each message was received - you should be seeing messages being continuously received during the present day/time.

    <img src=".//media/image90.png" width=800 />

21. Click on the **Properties** tab. Select one of the 3 options for the property `LED0` and click on the **Save** icon. Observe that the `LED0` on the AVR128DB48 Curiosity Nano Board behaves in the expected mode.

    <img src=".//media/image91.png" width=800 />

    Try changing the state of the `LED0` to switch between the Off, On, and Blinking modes. The Telemetry Reporting Rate can also be adjusted to change the period (in seconds) for sending the continuous telemetry reports.

22. Click on the **Commands** view. Type any text message in the "Message to echo" box and then click on the **Run** button. To see the response from the device, click on the **command history** link (located just underneath the **Run** button). You should see that the response was received "now" (i.e. within the last minute) and that the correct message was echoed from the device.

    <img src=".//media/image92.png" width=400 />
    <img src=".//media/image93.png" width=250 />

23. Click on the **Commands** view. Type `PT5S` in the "Delay before reboot operation" box and then click on the **Run** button. To see the response from the device, click on the **command history** link (located just underneath the **Run** button). You should see that the response was received "now" (i.e. within the last minute) and that a success message was received from the device.

    <img src=".//media/image94.png" width=400 />
    <img src=".//media/image95.png" width=200 />

    "PT5S" is an [ISO 8601](https://en.wikipedia.org/wiki/ISO_8601) period designator, which decodes to 5 seconds following the standard's definition.  This demo firmware only supports periods in seconds, but the standard covers durations of years, months, days, etc.

        'P'eriod 
        'T'ime 
         5
        'S'econds

    NOTE: You can access any of your IoT Central applications in the future by accessing the [IoT Central Portal](https://apps.azureiotcentral.com).

24. At this point, assuming everything is working as described, you can see if your USB hub can supply enough current to power both boards using just the single USB cable connected to the AVR128DB48 Curiosity Nano. Move the jumper on the RNWF02PC Add On Board so that the 2 pins furthest away from the module are shorted, and then disconnect the USB Type-C cable.

    <img src=".//media/RNWF02_Power_VCC.png" width=500/>

    Reset the AVR128DB48 Curiosity Nano by disconnecting and reconnecting the USB cable, or by holding down the `SW0` user button for approximately 4 seconds and then releasing it. Hopefully the connection is still successful!

    <img src=".//media/Terminal_Output.png" width=700/>

## References

Refer to the following links for additional information for IoT Explorer, IoT Hub, DPS, Plug and Play model, and IoT Central

•	[Manage cloud device messaging with Azure-IoT-Explorer](https://github.com/Azure/azure-iot-explorer/releases)

•	[Import the Plug and Play model](https://docs.microsoft.com/en-us/azure/iot-pnp/concepts-model-repository)

•	[Configure to connect to IoT Hub](https://docs.microsoft.com/en-us/azure/iot-pnp/quickstart-connect-device-c)

•	[How to use IoT Explorer to interact with the device](https://docs.microsoft.com/en-us/azure/iot-pnp/howto-use-iot-explorer#install-azure-iot-explorer)

•	[Azure IoT Central - All Documentation](https://docs.microsoft.com/en-us/azure/iot-central/)

•	[Create an Azure IoT Central application](https://docs.microsoft.com/en-us/azure/iot-central/core/quick-deploy-iot-central)

•	[Manage devices in your Azure IoT Central application](https://docs.microsoft.com/en-us/azure/iot-central/core/howto-manage-devices)

•	[How to connect devices with X.509 certificates for IoT Central](https://docs.microsoft.com/en-us/azure/iot-central/core/how-to-connect-devices-x509)

•	[Configure the IoT Central application dashboard](https://docs.microsoft.com/en-us/azure/iot-central/core/howto-add-tiles-to-your-dashboard)

•	[Customize the IoT Central UI](https://docs.microsoft.com/en-us/azure/iot-central/core/howto-customize-ui)

## Conclusion

You are now able to connect the [AVR128DB48 Curiosity Nano Evaluation Kit](https://www.microchip.com/en-us/development-tool/ev35l43a) to Azure IoT Central and should have deeper knowledge of how all the pieces of the puzzle fit together between Microchip's hardware and Microsoft's Azure Cloud services. Let’s start thinking out of the box and see how you can apply this project to provision securely and quickly a massive number of Microchip devices to Azure and safely manage them through the entire product life cycle.