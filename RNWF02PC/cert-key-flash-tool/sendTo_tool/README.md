### Overview
The CERT-KEY Flash tool can be used to load/flash certificates & keys on RNWF02PC board 

Note: To directly use source Python script, refer to [script](../python_script/)

### Features
1. One Click Installation & Uninstallation
2. Easy upload of Certificate & Keys using SendTo feature of Windows OS
3. SendTo "Flash" & "Delete" for up to 8 certificates or keys
4. Auto detects the COM/Serial Port of PIC32MZW2
5. Determines the File-type while uploading
6. Detects if file already exists

### Limitation
1. Compatible with Windows OS
2. Can be only used to load certificates and keys
   - ".crt & .pem" are uploaded as cert's
   - ".key" files are uploaded as keys

### Requirements
Microsoft Windows 8 and above

### Installation
Open Command Prompt and Run following command to install the tool

```
cd <Path_to_CERT-KEY_Flash_tool>\sendTo_tool
.\install.cmd
```

### Un-installation
Open Command Prompt and Run following command to un-install the tool

```
cd <Path_to_CERT-KEY_Flash_tool>\sendTo_tool
.\uninstall.cmd
```

### Usage
Right Click on up to 8 selected Certificates or Key files and select SendTo "CERT-KEY Flash"

Right Click on up to 8 selected Certificates or Key files and select SendTo "CERT-KEY Delete"

Right Click on any file and select SendTo "CERT-KEY List"


<p align="center"><img src="./images/sendTo2.png">
      </p>