# Provisioning a Microchip Curiosity Nano Evaluation Kit plus RNWF02PC Add On Board for Microsoft Azure IoT Services

## Introduction

This document describes how to connect a Microchip Curiosity Nano Evaluation Kit to a cloud application running on Microsoft's Azure IoT Central platform. Secure connections are made possible by using Certificate Authority (CA) signed X.509 certificate authentication between the Azure server and client (a.k.a. "device"). Wireless connectivity to the cloud is made possible by connecting Microchip's RNWF02PC module to a Host MCU standard UART port to serve an easy-to-use, serial-to-cloud bridge using AT commands.

The RNWF02 Early Access Development Kit is an efficient, low-cost development platform to evaluate and demonstrate the features and functionalities of Microchip’s low-power Wi-Fi® RNWF02PC Module. It can be used with a Host PC via USB Type-C® without the need of an additional hardware accessory. This is compliant to the mikroBUS™ Standard. The Add On Board can be plugged easily on the host board and can be controlled by the host Microcontroller Unit (MCU) with AT commands through UART.

The RNWF02 Early Access Development Kit offers:

- Easy-to-use platform to speed up design concepts to revenue with the low-power Wi-Fi RNWF02PC Module:

    - Host PC via USB Type-C interface
    - Host board supporting mikroBUS socket

- RNWF02PC Module, which includes a crypto device for a secure and authenticated cloud connection

- RNWF02PC Module mounted on the RNWF02 Add On Board as a pre-programmed device

## Hardware Requirements

* [USB-to-UART Serial Adapter/Bridge/Converter Cable](https://www.newark.com/c/cable-wire-cable-assemblies/cable-assemblies/usb-adapter-cables?conversion-type=usb-to-uart-converter)

    <img src=".//media/MFG_TTL-232R-RPI.jpg" width=150/>

* Host MCU Development Board: [AVR128DB48](https://www.microchip.com/en-us/development-tool/ev35l43a) or [PIC18F57Q84](https://www.microchip.com/en-us/development-tool/dm182030) Curiosity Nano Evaluation Kit

    <img src=".//media/boards/avr128db48_cnano.png" width=400/>
    <img src=".//media/boards/pic18f57q84_cnano.jpg" width=400/>

* [Curiosity Nano Base for Click Boards](https://www.microchip.com/en-us/development-tool/ac164162)

    <img src=".//media/boards/cnano_base_click.png" width=300/>

* Serial-to-Cloud Bridge Accessory Board: [RNWF02 Early Access Development Kit](https://www.microchip.com/en-us/development-tool/EA32M85A)

    <img src=".//media/boards/RNWF02PC_Add_On_Board.png" width=300/>

## Hardware Preparation

1. Install a jumper to short the two pins closest to the RNWF02PC module.

    <img src=".//media/boards/RNWF02_Power_USB.png" width=500/>

2. Provide dedicated power to the RNWF02PC Add On Board by connecting a USB Type-C cable to a +5V DC power source that's preferably not a USB port/hub (e.g. USB wall charger).

    <img src=".//media/boards/DC_Supply_RNWF02.png" width=500/>

3. Connect the USB-to-UART converter's RXD, TXD, & GND wires to the the TX, RX, & GND pins of the RNWF02PC Add On Board, respectively.

    <img src=".//media/boards/USB-UART_mikroBUS.png" width=500/>

4. Connect the USB end of the USB-to-UART serial converter cable to one of the PC's standard USB ports. This is how the Host PC will communicate with the RNWF02PC module for provisioning purposes.

    <img src=".//media/boards/PC_to_USB-UART.png" width=300/>

## Software Prerequisites & Installation

1. [Git](https://git-scm.com/)
2. [Python 3.0](https://www.python.org/download/releases/3.0/)
3. Any [Terminal Emulator](https://en.wikipedia.org/wiki/List_of_terminal_emulators) program of your choice
4. Microchip `MPLAB X` tool chain for embedded code development on 8-bit architecture MCU platforms (IDE & compiler)

    - [MPLAB X IDE](https://www.microchip.com/mplab/mplab-x-ide)

    - [MPLAB XC8 Compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers#tabs)

        NOTE: This demonstration project was last tested successfully with XC8 v2.45, and in general should work with later versions of the compiler as they become available. If you encounter issues building the project with a newer version of the compiler, it is recommended to download the last known working version from the [MPLAB Development Ecosystem Downloads Archive](https://www.microchip.com/en-us/tools-resources/archives/mplab-ecosystem) (to fall back to the version Microchip successfully tested prior to release). 


## Device Provisioning & Cloud Resources Setup

### Step 1 - Clone this Repository

Create a clone of this repository using the [Git](https://git-scm.com) tool in a command line window

```bash
git clone https://github.com/MicrochipTech/AzureDemo_RNWF
```

As an alternative, scroll up towards the top of this page, click on the **Code** button, and download a ZIP file of this repository

<img src=".//media/Download_ZIP.png" width=400/>

### Step 2 - Provision the RNWF02PC Module

For secure connections, a chain of trust (which includes certificates & keys for the root, signer, and client) needs to be generated and programmed into the RNWF02PC module.

The device (client) certificate file will be needed when we create the device in Azure IoT Central using the individual enrollment method. Another option is to use the group enrollment method which requires uploading the signer certificate file (or in some cases, could be the root certificate if the client certificate is chained directly to the Root CA) to the Azure IoT Central application, so that any device which presents a leaf certificate that was derived from the signer (or root) certificate will automatically be granted access to registration (which is governed by the Device Provisioning Service linked to the IoT Hub that's used by an IoT Central application).

#### 2.1 Generate the Files for Your Chain of Trust

To automate the process, GitBash shell scripts are provided to generate your chain of trust. During execution of the scripts, you will be asked to provide names for the Root CA, Signer (Subordinate/Intermediate) CA, and the Common Name (CN) for the device. The Common Name for the device should be a unique identifier and will be used as the registration ID when connecting to the Azure IoT Central's Device Provisioning Service (DPS).

Click [here](./cert-chain-gen-scripts/) and follow the instructions to use the GitBash shell scripts to generate your chain of trust. 

#### 2.2 Upload Device Certificate & Key into the RNWF02PC Module

A Python script has been provided to help you easily program the device's certificate & key files into the RNWF02PC module.

1. Open a `Command Prompt` window and use the `cd` command to navigate to the `\AzureDemo_RNWF\RNWF02\cert-key-flash-tool\python_script` directory.

2. Execute the following command to install the Python packages required for executing the script (if `pip` is an unrecognized command, try using `pip3`):

    ```bash
    pip install -r requirements.txt
    ```

3. Execute the following command to get a list of all the **certificates** that are currently programmed in the RNWF02PC module (if `python` is an unrecognized command, try using `python3` or `py -3`):

    ```bash
    python file_transfer.py list cert
    ```

4. Load the device **certificate** file (e.g. `<Common_Name>.pem`) into the RNWF02PC module by executing the following command line (and which uses the Common Name (CN) as the name of the **certificate** file stored in the module):

    ```bash
    python file_transfer.py load cert -f "<Common_Name>" -p "../../cert-chain-gen-scripts/devcerts/<Common_Name>/<Common_Name>.pem"
    ```

    For example, to program the example device certificate (Common Name = "rnwf02_device_01") that has been provided, the exact command line to execute would be:

    ```bash
    python file_transfer.py load cert -f "rnwf02_device_01" -p "../../cert-chain-gen-scripts/devcerts/rnwf02_device_01/rnwf02_device_01.pem"
    ```

5. Execute the following command to get a list of all the **keys** that are currently programmed in the RNWF02PC module  (if `python` is an unrecognized command, try using `python3` or `py -3`):

    ```bash
    python file_transfer.py list key
    ```

6. Load the device **key** file (e.g. `<Common_Name>.key`) into the RNWF02PC module by executing the following command line (and which uses the Common Name (CN) as the name of the **key** file stored in the module):

    ```bash
    python file_transfer.py load key -f "<Common_Name>" -p "../../cert-chain-gen-scripts/devcerts/<Common_Name>/<Common_Name>.key"
    ```

    For example, to program the example device key that has been provided, the exact command line to execute would be:

    ```bash
    python file_transfer.py load key -f "rnwf02_device_01" -p "../../cert-chain-gen-scripts/devcerts/rnwf02_device_01/rnwf02_device_01.key"
    ```

7. If needed in the future for maintenance purposes, previously-programmed certificates and/or keys can be deleted from the RNWF02PC module by executing the following commands:

    ```bash
    python file_transfer.py delete cert -f "<CERTIFICATE_NAME>"
    python file_transfer.py delete key -f "<KEY_NAME>"
    ```

8. Upon successful completion of uploading the device certificate & key files into the RNWF02PC module, the USB-to-UART converter can now be disconnected from the PC & RNWF02PC Add On Board. Keep the RNWF02PC Add On Board powered on via the USB Type-C cable.

### Step 3 - Create an Azure IoT Central Cloud Application

#### 3.1 Create an Azure Account and Subscription

Microsoft has excellent instructions to create an new Azure account and subscription.  Please create an account and subscription before continuing. Review our overview of the [Azure Account/Subscription creation process](./CreateAzureAccountAndSubscription.md) as required.

#### 3.2 Use the Azure IoT Central Application Builder

Click [here](CreateAnIoTCentralApplication.md) and follow the procedure to create an IoT Central application to be used with this demonstration.

NOTE: You can access any of your IoT Central applications in the future by signing into the [IoT Central Portal](https://apps.azureiotcentral.com).

### Step 4 - Enroll Your Device in the Azure IoT Central Application

- Access your IoT Central application by signing into the [IoT Central Portal](https://apps.azureiotcentral.com), clicking on `My Apps` in the left-hand side navigation pane, and then clicking on the tile that is labeled with the name of your application.

    <img src="./media/IOTC_MyApps.png" alt="Script Configuration" width = 250/>

- Click on one of the following procedures to pre-register your device with the IoT Central application (creating an enrollment group is strongly recommended as a more practical solution vs. registering each device individually):

    - [Create an Enrollment Group](./IoT_Central_Group_Enrollment.md)
    - [Create an Individual Enrollment](./IoT_Central_Individual_Enrollment.md)

**For Your Education**: [Group enrollment](https://learn.microsoft.com/en-us/azure/iot-dps/concepts-service#enrollment-group) allows you to create a group of allowable devices which each have a leaf certificate derived from a common signer or root certificate so that devices do not need to be pre-enrolled on an individual basis. Enrollment groups are used to enroll multiple related devices; [Individual enrollment](https://learn.microsoft.com/en-us/azure/iot-dps/concepts-service#individual-enrollment) is used to enroll a single device. Feel free to review both methods and be sure to complete the procedure for your preferred method before proceeding with the next step.

## Test Device Interaction with the Cloud Application

Click on one of the following test procedures for the specific Curiosity Nano Evaluation Kit currently being used for this demonstration:

* [AVR128DB48](./AVR128DB48_RNWF02PC.md)
* [PIC18F57Q84](./PIC18F57Q84_RNWF02PC.md)

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

You are now able to connect one of Microchip Technology's Curiosity Nano Evaluation Kits to Azure IoT Central and should have deeper knowledge of how all the pieces of the puzzle fit together between Microchip's hardware and Microsoft's Azure Cloud services. Let’s start thinking out of the box and see how you can apply this project to provision securely and quickly a massive number of Microchip devices to Azure and safely manage them through the entire product life cycle.