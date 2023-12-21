### Overview
This script is to be used to read Serial number and Device certificate of your device.

### Usage
Connect RNWF11 board to your PC. Open RNWF11_ClientCert.py on Text Editor of your choice. In the top part of the code locate the line:
COM_PORT = "Your COM port"
Replace "Your COM port" with USB Serial Device COM port. For example, if your Device Manager shows USB Serial Device connected on COM21 then your line will look like:
COM_PORT = "COM21"
Save the changes and run the following command in Command Prompt:
python RNWF11_ClientCert.py

The Device Certificate is stored under the Common Name (Serial Number) of device in the same folder in the format <serial_number>.crt. The user can also see the Serial Number printed on the console.

### Read Signer certificate
To read Signer certificate from the device instead of Device Certificate, locate following line in the python script:
self.cmd_issue('AT+ECCRDCERT=1,1500\r\n')
Replace the above mentioned line with following line:
self.cmd_issue('AT+ECCRDCERT=2,1500\r\n')
Save the changes and run the python file as before:
python RNWF11_ClientCert.py

The Signer Certificate will be stored under the Common Name (Serial Number) of the device in the format <serial_number>.crt. 

Note: To differentiate between Device Certificate and Signer Certificate, simply open the .crt file and check for "Issued to" name in it. For Signer Certificate 'Issued To' will be prefixed with 'Crypto Authentication Signer', whereas for Device Certificate 'Issue To' will be the Common Name of the device.