### Overview
This script can be used to load, delete & list certificates and keys on PIC32MZW2 board from command line

### Features
1. Auto detects the COM/Serial Port of PIC32MZW2
2. Determines the File-type while uploading
3. Detects if file already exists

### Limitation
1. It currently supports PEM Certificate and Keys

### Requirements
1. Install Python 3.8.0+
2. Install required libraries

```
pip install -r requirements.txt
```

### Usage
```
python file_transfer.py <Operation> [-h] [-f <File_Name>] [-p <File_Path>] [-proto Protocol]

positional arguments:
  Operation             Allowed values ['list','load', 'delete']
  File_Type             Allowed values ['cert','key'] (Supported only for list & delete)

optional arguments:
  -h, --help                            show this help message and exit
  -f <File_Name>, --file <File_Name>    File Name to save on board
  -p <File_Path>, --path <File_Path>    File Path on local machine
  -proto Protocol, --protocol Protocol  File transfer protocol ('xsum','xcrc','x1k' Default:xsum)
```

### Examples
1. List Certificates or Keys
```
python file_transfer.py list cert 
OR 
python file_transfer.py list key
```

2. Load Certificate or Key
```
python file_transfer.py load cert -f <File Name to save on Board> -p <File Path on local machine>
OR
python file_transfer.py load key -f <File Name to save on Board> -p <File Path on local machine>
```

3. Delete Certificate or Key
```
python file_transfer.py delete cert -f <File Name to delete from Board>
OR 
python file_transfer.py delete key -f <File Name to delete from Board>
```

4. Specify Protocol while uploading the Certificate or Key
```
python file_transfer.py load cert/key -f <File Name to save on Board> -p <File Path on local machine> -proto xcrc
```
