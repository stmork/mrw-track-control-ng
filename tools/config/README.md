# The MRW-Configure tool
The MRW-Configure tool configures the model railways devices it should control. Up to 44 devices are allowed. To configure the CAN controllers a model railway needs to be read.

## Program flow
Each controller is handled seperately. There occure no broadcast sending.1. To configure a CAN controller it has to enter the configuration by sending an addressed CFGBGN (config begin) command.
2. All devices to be configured have their own CFG command. Up to 44 devices are allowed. By repeatedly sending these CFG commands the CAN controller collects the device configuration.
3. The configuration mode ends with a final CFGEND (config end) command. The data is written into enternal EEPROM which takes about 150 ms for each device. Yes, EEPROM writing is slow!
3. After writing the device configuration into EEPROM the controller sends a CFGEND / MSG_OK response and additionally the amount of devices configured.
5. After completing the device configuration the normal firmware execution starts resulting in RESET / MSG_BOOTED and GETVER / MSG_OK responses.

This diagram shows the following sequence diagram:

```mermaid
sequenceDiagram
	Participant T as Tool
	Participant F as Firmware

	Note right of T: CAN node collection is not needed because the model knows the amount.
	T ->> +F: CFGBGN
	F ->> -T: CFGBGN: MSG_OK

	loop until all devices configured
		T ->> +F: CFGxyz
		F ->> -T: CFGxyz: MSG_OK
	end

	Note right of T: Flashing is complete now validate checksum.
	T ->> +F: CFGEND

	F ->> -T: CFGEND: MSG_OK

	activate F
	Note right of T: Ressetting occurs.

	F ->> +T: GETVER: MSG_OK
	F ->> -T: RESET: MSG_BOOTED

	Note left of F: Firmware has restarted.
```

The internal behaviour is controlled by the following statechart:

<img src="../../statecharts/images/ConfigStatechart_0.png"/>
