# Creating an Enrollment Group for an IoT Central Application

1. Using the left-hand side navigation pane of your IoT Central application, select `Security` &gt; `Permissions` &gt; `Device connection groups`

   <img src=".//media/image81a.png" width = 250 />

2. Click on the `+ New` button and create a new enrollment group using any name (with Group type = `IoT devices` and attestation type = `Certificates (X.509)`).  Hit the `Save` icon when finished

   <img src=".//media/image81b.png" width = 400 />

3. Now that the new enrollment group has been created, click on `Manage Primary`

   <img src=".//media/image82.png" width = 400 />

4. Click on `+ Add certificate` and browse to the **signer** certificate file (e.g. `subca.crt`). Click the `Upload` button (then click on `Close` when the certificate has been accepted)

   <img src=".//media/image76.png" width = 400 />

5. Optionally, a secondary root or intermediate certificate can be uploaded as well

   <img src=".//media/image77.png" width = 400 />

6. Click on the `Save` icon at the top of the page, and note the ID Scope which was created for the enrollment group. Now, any leaf certificate that was derived from the signer certificate will be allowed to automatically enroll with the DPS ID scope associated with the IoT Central application. The X.509 enrollment group has been successfully created and should be ready to go!

    <img src=".//media/image78.png" width = 400 />
