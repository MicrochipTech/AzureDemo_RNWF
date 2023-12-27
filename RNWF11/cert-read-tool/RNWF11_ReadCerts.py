import subprocess

subprocess.run('python RNWF11_ReadCert_Signer.py', shell=True, check=False)
subprocess.run('python RNWF11_ReadCert_Client.py', shell=True, check=False)

subprocess.run('python3 RNWF11_ReadCert_Signer.py', shell=True, check=False)
subprocess.run('python3 RNWF11_ReadCert_Client.py', shell=True, check=False)

