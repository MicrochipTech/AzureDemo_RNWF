# Provisioning a Microchip Curiosity Nano Evaluation Kit plus RNWF11PC Add On Board for Microsoft Azure IoT Services

## Introduction

This document describes how to connect a Microchip Curiosity Nano Evaluation Kit to a cloud application running on Microsoft's Azure IoT Central platform. Secure connections are made possible by using Certificate Authority (CA) signed X.509 certificate authentication between the Azure server and client (a.k.a. "device"). Wireless connectivity to the cloud is made possible by connecting Microchip's RNWF11PC module to a Host MCU standard UART port to serve an easy-to-use, serial-to-cloud bridge using straightforward AT commands.

The RNWF UART to Cloud Add On Board is an efficient, low-cost development platform to evaluate and demonstrate the features and functionalities of Microchip’s low-power Wi-Fi® RNWF11PC Module. It can be used with a Host PC via USB Type-C® without the need of an additional hardware accessory. This is compliant to the mikroBUS™ Standard. The Add On Board can be plugged easily on the host board and can be controlled by the host Microcontroller Unit (MCU) with AT commands through UART.

The RNWF UART to Cloud Add On Board offers:

- Easy-to-use platform to speed up design concepts to revenue with the low-power Wi-Fi RNWF11PC Module:

    - Host PC via USB Type-C interface
    - Host board supporting mikroBUS socket

- RNWF11PC Module, which includes a crypto device for a secure and authenticated cloud connection that has been pre-provisioned with X.509 certificates and private keys

- RNWF11PC Module mounted on the RNWF UART to Cloud Add On Board as a pre-programmed device

## Hardware Requirements

* Host MCU Development Board: [AVR128DB48 Curiosity Nano](https://www.microchip.com/en-us/development-tool/ev35l43a)

    <img src=".//media/boards/avr128db48_cnano.png" width=400/>

* [Curiosity Nano Base for Click Boards](https://www.microchip.com/en-us/development-tool/ac164162)

    <img src=".//media/boards/cnano_base_click.png" width=300/>

* Serial-to-Cloud Bridge Accessory Board: [RNWF UART to Cloud Add On Board]()

    <img src=".//media/boards/drawings/RNWF_Add_On_HQ [Horizontal].png" width=400/>

## Software Prerequisites & Installation

Click on each of the following links to install the specified software component:

1. [Git](https://git-scm.com/downloads) distributed version control system

2. Latest release of [Python](https://www.python.org/downloads/) (during the installation setup sequence (if offered as an option), check the box to add python.exe to the PATH environment variable)

    <img src=".//media//Python_PATH.png" width=400/>

    Once installation has completed, launch a Command Prompt (or PowerShell) window to confirm that one of `python` or `python3` is a recognized operation when entered on the command line; e.g. check the currently installed version of Python

    <img src=".//media//Python_version_CP.png" width=250/>
    <img src=".//media//Python_version_PS.png" width=335/>

    In some Windows environments, the Python operation may need to be executed as "py -3"

    <img src=".//media//py3_version_CP.png" width=315/>
    <img src=".//media//py3_version_PS.png" width=335/>

    NOTE: If the command line fails to print out the Python version number, click [here](https://realpython.com/add-python-to-path/) and follow the directions on adding the location of the Python executable to the PATH environment variable used by your Operating System

3. [PIP](https://www.geeksforgeeks.org/how-to-install-pip-on-windows/) Package Management System for Python

4. Any [Terminal Emulator](https://en.wikipedia.org/wiki/List_of_terminal_emulators) program of your choice

5. Microchip `MPLAB X` tool chain for embedded code development on 8-bit architecture MCU platforms (IDE & compiler)

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

### Step 2 - Chain of Trust for Secure Cloud Connections

For enabling secure connections with cloud services such as AWS and Azure, a chain of trust (which includes certificates & keys for the root, signer, and client) has been pre-provisioned (programmed) into the RNWF11PC module.

The device (client) certificate file will be needed when we create the device in Azure IoT Central using the individual enrollment method. Another option is to use the group enrollment method which requires uploading the signer certificate file (or in some cases, could be the root certificate if the client certificate is chained directly to the Root CA) to the Azure IoT Central application, so that any device which presents a leaf certificate that was derived from the signer (or root) certificate will automatically be granted access to registration (which is governed by the Device Provisioning Service linked to the IoT Hub that's used by an IoT Central application).

#### 2.1 Install a jumper on the 3-pin header (most likely labeled `J5`on the silkscreen) to short the two pins that are **furthest** away from the `PWR` LED on the RNWF UART to Cloud Add On Board. The middle pin should be shorted with the pin labeled `PC3V3` towards the right side of the board. This jumper setting configures the board to draw power directly from the USB Type-C connector.

<img src=".//media/boards/RNWF11_Power_USB.png" width=400/>

#### 2.2 Use a USB Type-C cable to connect the RNWF UART to Cloud Add On Board to the Host PC. Confirm that the `PWR` LED on the board stays constantly on - signifying that the board is getting sufficient power.

<img src=".//media/boards/RNWF11_to_PC.png" width=300/>

#### 2.3 Launch a Command Prompt or PowerShell window (ideally as an Administrator) and execute the following 2 command lines in order to remove any previous installation of the `pyserial` Python package and then reinstall the latest package (if `pip3` is not a recognized command, try `pip`):

```bash
pip3 uninstall pyserial
pip3 install pyserial
```

NOTE: If neither `pip` nor `pip3` is recognized as a valid executable on the command line, click [here](https://phoenixnap.com/kb/install-pip-windows)

#### 2.4 Both the **SIGNER** and **CLIENT** X.509 certificates can be easily read out of the RNWF11 module by executing a Python script located in the [cert-read-tool](./cert-read-tool/) sub-directory. Execute the following command in a PowerShell or Command Prompt window (if `python3` is not a recognized command, try `python`):

    python3 RNWF11_ReadCerts.py

**Note** If the development board is not responding to the script's commands, kill the python operation, power cycle the development board, and re-run the script

#### 2.5 Note and write down the "Common Name" (aka "Device ID") that was generated as part of the names of the PEM files in the [cert-read-tool](./cert-read-tool/) sub-directory (e.g. "sn01237348B762507701").

<img src=".//media/Common_Name_PEM.png" width=300/>

This "Device ID" is basically the name of the device that is generated based on a unique serial number that is pre-programmed in a secure element that's built into the RNWF11 module. It is also equal to the Common Name that is used in the Client X.509 certificate used for authentication. You will need to enter in this label as the "Device ID" value in the demonstration firmware in a future step.

#### 2.6 Disconnect the Type-C USB cable from the RNWF UART to Cloud Add On Board and then **move** the jumper so that it is shorting the two pins **closest** to the `PWR` LED. The middle pin of the 3-pin header should be shorted with the pin labeled `HOST3V3` towards the left side of the board.

<img src=".//media/boards/RNWF11_Power_VCC.png" width=400/>

### Step 3 - Create an Azure IoT Central Cloud Application

If you already have an existing IoT Central Application created, skip to Step 4.

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

* [AVR128DB48 Curiosity Nano](./AVR128DB48_RNWF11PC.md)

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