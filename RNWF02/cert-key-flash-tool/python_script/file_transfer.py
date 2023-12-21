import serial
import argparse
from xmodem import XMODEM
from time import sleep
import io
import os
import serial.tools.list_ports as list_ports

# Possible VID:PID combinations for FTDI vendors:devices
dev_dict = [    
                '0403:6001',
                '0403:6010',
                '0403:6011',
                '0403:0014',
                '0403:0015',
                '0403:A9A0',
                '04D8:00DD',
                '04D8:00DF',
                '04D8:810B',
                '10C4:80A9',
                '10C4:EA60',
                '10C4:EA61',
                '10C4:EA63',
                '10C4:EA70',
                '10C4:EA71',
                '10C4:EA80',
                '067B:2303',
                '067B:AAA2',
                '067B:AAA3'
]

version = '1.0.0'

welcome=(
    "\n********************************************************************************"
    "\n***************                                                  ***************"
    "\n***************            Microchip Technology Inc.             ***************"     
    "\n***************         PIC32MZW2 CERT-KEY Flash Utility         ***************"
    f"\n**************                    v{version}                    ***************"
    "\n***************                                                  ***************"
    "\n********************************************************************************"
)

# Display all active serial ports
def all_active_serial_ports():
    ports = list_ports.comports()
    ftdi_ports = []
    for port, desc, hwid in sorted(ports):
        for VID_PID in dev_dict:
            index = hwid.find(VID_PID)
            if index != -1:
                ftdi_ports.append(port)
    return detect_port(ftdi_ports)

# Find out the FTDI port number for PIC32MZW2
def detect_port(ports):
    for port in ports:
        output = []
        try:
            s = serial.Serial(port=port, baudrate=230400, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=4.0, write_timeout=4.0, inter_byte_timeout=0.5)
            sleep(0.1)
            s.write(f'AT+GMM\r\n'.encode('UTF-8'))
            sleep(0.1)
            try:
                while s.in_waiting:
                    if s.in_waiting == 1:
                        break
                    c = s.readline()
                    c = c.decode('UTF-8').strip('\r\n').replace('+GMM:', '')
                    output.append(c)
            except UnicodeDecodeError:
                pass
            if len(output) != 0:
                if output[1] == '"PIC32MZW2"' and output[2] == 'OK':
                    s.close()
                    return port
            s.close()
        except serial.SerialException:
            pass
    print('Port not detected')
    raise SystemExit(1)

# Clean the serial response from PIC32MZW2
def pretty(command, output, file):
    output = output.decode('UTF-8')
    output = output.replace(command,'')
    output = output.replace(file,'')
    output = output.split(',')
    output = output[-1].strip('\n\r\">')
    return output

# certificate/key LOAD operation
def load(fType, txproto, file, path):
    def getc(size, timeout = 1):
        return s.read(size)
    def putc(data, timeout = 1):
        return s.write(data)
    modem = XMODEM(getc, putc)

    try:
        os.path.exists(path)
    except FileNotFoundError:    
        raise SystemExit(1)

    with open(f'{path}', 'rb') as f:
        line = f.readline().decode('UTF-8')
        if line.find('CERTIFICATE') != -1:
            file_type = '1'
        elif line.find('KEY') != -1:
            file_type = '2'
        elif fType == '1':
            file_type = 1
        elif fType == '2':
            file_type = 2
        else:
            print('\nERROR: File Type Not Supported')
            raise SystemExit(1)
    
    ftype = 'Certificate' if file_type == '1' else 'Key'
    if file in list(file_type, False):
        list(file_type, True)
        print(f'\nERROR: {ftype} {file} already exist!')
        raise SystemExit(1)

    with open(f'{path}', 'rb') as f:
        data = f.read()
        length = str(len(data))
        s.write(f'AT+FS=1,{file_type},{txproto},"{file}",{length}\r\n'.encode('UTF-8'))
        sleep(0.5)
        while s.in_waiting:
            c = s.read()

        data = io.BytesIO(data)
        try:
            status = modem.send(data, retry=1)
        except Exception as e:
            print(e)
        if status:
            list(file_type, True)
            print(f'\n{ftype} "{file}" uploaded successfully!\n')
        else:
            print(f'\nERROR: {ftype} "{file}" upload failure')

# certificate/key LIST operation
def list(file_type, to_print):
    command = f'AT+FS=2,{file_type}'
    s.write(f'{command}\r\n'.encode('UTF-8'))
    sleep(0.1)
    ftype = 'Certificates' if file_type == '1' else 'Keys' 
    print(f'\n\nList of {ftype}')
    print('-' * 20)
    idx = 1
    file_list = []
    while s.in_waiting:
        if s.in_waiting == 1:
            break
        c = s.readline()
        response = pretty(command, c, '')
        if response != '' and response != 'OK':
            if to_print:
                print(f'{idx} : {response}')
            file_list.append(response)
            idx = idx+1
    s.close
    return file_list

# certificate/key DELETE operation
def delete(file_type, file):
    command = f'AT+FS=3,{file_type}'
    ftype = 'Certificate' if file_type == '1' else 'Key'
    s.write(f'{command},"{file}"\r\n'.encode('UTF-8'))
    sleep(0.5)
    while s.in_waiting:
        if s.in_waiting == 1:
            break
        c = s.readline()
        response = pretty(command, c, file)
        if response == 'OK':
            list(file_type, True)
            print(f'\n{ftype} "{file}" deleted successfully!\n')
        elif response == 'File Not Found':
            print(f'\nERROR: {ftype} "{file}" not found!')
        elif response != '':
            print(f'\nERROR:{ftype} "{file}" failed to delete!')

# Validate arguments
def validate_args(args, parser):
    if args.op.lower() == 'load':
        if args.type == None or args.file == None or args.path == None:
            load_error=('Invalid Arguments for load command\n\nExample:\npython file_transfer.py '
                        'load cert/key -f <File Name to save on Board> -p <File Path on local machine>')
            print('\n')
            parser.error(load_error)

    if args.op.lower() == 'delete':
        if args.type == None or args.file == None or not args.path == None:
            delete_error=('Invalid Arguments for delete command\n\nExample:\npython file_transfer.py '
                        'delete cert/key -f <File Name to delete from Board>')
            print('\n')
            parser.error(delete_error)
    
    if args.op.lower() == 'list':
        if args.type == None or not args.file == None or not args.path == None:
            list_error=('Invalid Arguments for list command\n\nExample:\npython file_transfer.py '
                        'list cert/key')
            print('\n')
            parser.error(list_error)


#TODO: Add logic to validate number of arguments
def get_args():
    description='Python script to manage Certificates & Keys on PIC32MZW2 board'
    script_usage='\npython file_transfer.py <Operation> <File_Type> [-h] [-f <File_Name>] [-p <File_Path>] [-proto Protocol]'
    parser = argparse.ArgumentParser(description=description, usage=script_usage)
    
    parser.add_argument("op", metavar='Operation', choices=['list','load','delete'], help="Allowed values ['list','load','delete']")
    parser.add_argument("type", metavar='File_Type', nargs='?', choices=['cert','key'], help="Allowed values ['cert','key'] (Supported for load, list & delete)")
    parser.add_argument("-f", "--file", metavar='<File_Name>', help="File Name to save on board")
    parser.add_argument("-p", "--path", metavar='<File_Path>', help="File Path on local machine")
    parser.add_argument("-proto", "--protocol", metavar='Protocol', default='xsum', choices=['xsum','xcrc','x1k'], help="File transfer protocol ('xsum','xcrc','x1k' Default:xsum)")
    args = parser.parse_args()

    validate_args(args, parser)

    if args.type:
        if args.type.lower() == 'cert':
            args.type = '1'
        elif args.type.lower() == 'key':
            args.type = '2'
    
    if args.protocol.lower() == 'xsum':
        args.protocol = '1'
    elif args.protocol.lower() == 'xcrc':
        args.protocol = '2'
    if args.protocol.lower() == 'x1k':
        args.protocol = '3'

    return args

if __name__ == "__main__":
    print(welcome)
    args=get_args()
    COMMAND=args.op.lower()
    PORT = all_active_serial_ports()
    print(f'\n\nIdentified PIC32MZW2 Port')
    print('-' * 20)
    print(PORT)
    print('-' * 20)
    s = serial.Serial(port=PORT, baudrate=230400, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=None)
    if COMMAND == 'load':
        load(args.type,args.protocol,args.file,args.path)
    elif COMMAND == 'list':
        list(args.type, True)
    elif COMMAND == 'delete':
        delete(args.type,args.file)