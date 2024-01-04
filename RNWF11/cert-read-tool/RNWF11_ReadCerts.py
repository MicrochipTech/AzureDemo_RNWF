import subprocess
import platform

if (platform.system() == 'Windows'):
  subprocess.run('py -3 RNWF11_ReadCert_Signer.py', shell=True, check=False)
  subprocess.run('py -3 RNWF11_ReadCert_Client.py', shell=True, check=False)
else:
  subprocess.run('python3 RNWF11_ReadCert_Signer.py', shell=True, check=False)
  subprocess.run('python3 RNWF11_ReadCert_Client.py', shell=True, check=False)
