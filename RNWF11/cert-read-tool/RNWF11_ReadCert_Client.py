import serial
import time
import subprocess
import io
import os
from time import sleep
import serial.tools.list_ports as list_ports

RNWF11_BAUDRATE = 230400

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

# Find out the FTDI port number for RNWFxx
def detect_port(ports):
    for port in ports:
        output = []
        try:
            s = serial.Serial(port=port, baudrate=RNWF11_BAUDRATE, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=4.0, write_timeout=4.0, inter_byte_timeout=0.5)
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
                #RNWF02
                if (output[1] == '"PIC32MZW2"') or (output[1] == '"RNWF02"') and output[2] == 'OK':    
                    s.close()
                    return port
                #RNWF11
                elif (output[1] == '"PIC32MZW1"') or (output[1] == '"RNWF11"') and output[2] == 'OK':
                    s.close()
                    return port
            s.close()
        except serial.SerialException:
            pass
    print('Port not detected')
    raise SystemExit(1)

# Clean the serial response from RNWFxx
def pretty(command, output, file):
    output = output.decode('UTF-8')
    output = output.replace(command,'')
    output = output.replace(file,'')
    output = output.split(',')
    output = output[-1].strip('\n\r\">')
    return output

class Delay_Non_Blocking:
  def __init__(self):
    self.isStarted = False
    self.time_start = 0

  def delay_time_start(self):
    if self.isStarted == False:
      self.time_start = time.time()
      self.isStarted = True
    
  def delay_sec_poll(self, delay_sec):
    if time.time() - self.time_start > delay_sec :
      self.isStarted = False
      return True
    else:
      return False

class ReadClientCert:
  def __init__(self, port, baud, debug):
    self.PORT = port
    self.BAUD = baud
    self.DEBUG = debug
    self.SER_TIMEOUT = 0.1
    self.ser_buf = ""
    self.init_state = 0
    self.app_state = 0
    self.dev_cert = ""
    self.wifi_connected = False
    self.ser = serial.Serial(self.PORT, self.BAUD, timeout = self.SER_TIMEOUT)
    self.delay = Delay_Non_Blocking()
    self.evt_handler = None
  def cmd_issue(self, command):
    self.ser.write(bytearray(command, 'utf-8'))
  
  def serial_receive(self):
    read_val = self.ser.read(1)
    if read_val != b'':
      if self.DEBUG == True:
        print(read_val)
      self.ser_buf = self.ser_buf + read_val.decode('utf8', errors='backslashreplace')
      if read_val == b'>':
        ret_val = self.ser_buf
        self.ser_buf = ""
        return ret_val
    return ""
  
  def sm_initialize(self):
      # start initialization
      if self.init_state == 0:
        print("\r\nStarting...\r\n")
        self.init_state = 1
      self.delay.delay_time_start()
      if self.delay.delay_sec_poll(0.100) :
        #enable serial command echo
        if self.init_state == 1:
          self.cmd_issue('ATE1\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
          
        if self.init_state == 2:
          self.cmd_issue('AT+ECCRDSER\r\n')
          self.init_state = self.init_state + 1
          return self.init_state
                        
        if self.init_state == 3:
          self.cmd_issue('AT+ECCRDCERT=1,1500\r\n')
          self.init_state = 10
          return self.init_state
        #delay until event changes init_state
        elif self.init_state == 10: 
          # don't advance state automatically, do it from evt_handler
          pass
        
        # init complete        
        elif self.init_state == 254:
          return self.init_state          
        else:
          return 255
      else:
        return 0
    
  def evt_init_error(self):
    self.init_state = 254
    print("Event: Error,stopping initialization")
  
  def evt_read_ser(self) :
    #print(len(self.read_ser))
    first_pos = self.read_ser.find('"')
    second_pos = len(self.read_ser)-8
    global cn
    cn = self.read_ser[first_pos+1:second_pos]
    cn = 'sn' + cn
    #print(cn)
    self.init_state = 3
    
  def evt_read_certificate(self) :
    cert_start = self.dev_cert.find("-----BEGIN CERTIFICATE-----")
    cert_end = self.dev_cert.find("-----END CERTIFICATE-----")+ len("-----END CERTIFICATE-----")
    #print("start: " + str(cert_start) + " end: "+ str(cert_end))
    self.dev_cert = self.dev_cert[cert_start:cert_end]
    #print(self.dev_cert)
    cert_list = self.dev_cert.rsplit("\\n")
    #print(cert_list)
    cert = ""
    for i in cert_list:
      cert = cert + i +'\r\n'
    #print(cert)
    f = open("Cert.pem", "w")
    f.write(cert)
    f.close()

    print("\r\n\r\nThe Common Name in the client certificate is: " + cn + "\r\n")
    
    if os.name == 'posix' :
      cmd_line = "mv Cert.pem " + cn + "_client.pem"
    else:
      cmd_line = "rename Cert.pem " + cn + "_client.pem"
    print(cmd_line)
    p = subprocess.Popen(cmd_line, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in p.stdout.readlines():
      print(line.rstrip().decode())
    
    self.init_state = 254 # end initialization.
  
  def rx_data_process(self, received):
    # if error setting echo on, bail
    if ("ATE1" in received) and ("ERROR:" in received) :
      self.evt_handler = self.evt_init_error
      retval = 0 #error state
      
    if ("+ECCRDSER:" in received):
        self.read_ser = received
        self.evt_handler = self.evt_read_ser
    
    if ("+ECCRDCERT:" in received):
      self.dev_cert = received
      self.evt_handler = self.evt_read_certificate
        
  def runApp(self):

    #top level app state machine
    if self.app_state == 0:  # start of application
      print("\r\n--------------------------------------------------------------------------------")
      print("Reading RNWF11 Client Certificate")
      print("--------------------------------------------------------------------------------\r\n")
      self.app_state = 1
    
    elif self.app_state == 1:  # initialization state
      init_resp = self.sm_initialize()
      if init_resp == 254 :
        self.app_state = 254

    elif self.app_state == 254:
      exit()
      
    rx_data = self.serial_receive()
    # parse received data
    if rx_data != "":
      print(rx_data)
      self.rx_data_process(rx_data)
      if self.evt_handler != None :
        self.evt_handler()
        self.evt_handler = None  

  def __del__(self):
    self.ser.close()

PORT = all_active_serial_ports()
print(f'\n\nIdentified RNWFxx Port')
print('-' * 20)
print(PORT)
print('-' * 20)
#s = serial.Serial(port=PORT, baudrate=RNWF11_BAUDRATE, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=None)

certificate = ReadClientCert(PORT, RNWF11_BAUDRATE, False)

while True:
  
  certificate.runApp()
