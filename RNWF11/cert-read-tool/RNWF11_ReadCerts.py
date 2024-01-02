import subprocess
 
def find_python(name):
  from shutil import which
  return which(name) is not None

print("\nLooking for the Python executable...")
if find_python("python3.exe") | find_python("python3"):
  print("The 'python3' executable was found")
  subprocess.run('python3 RNWF11_ReadCert_Signer.py', shell=True, check=False)
  subprocess.run('python3 RNWF11_ReadCert_Client.py', shell=True, check=False)
elif find_python("python.exe") | find_python("python"):
  print("The 'python' executable was found")
  subprocess.run('python RNWF11_ReadCert_Signer.py', shell=True, check=False)
  subprocess.run('python RNWF11_ReadCert_Client.py', shell=True, check=False)
else:
  print("Neither 'python' nor 'python3' was found as a valid executable in the environment's path")
