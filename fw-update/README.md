# The MRW-Update tool
The MRW-Update tool updates the firmware of all connected CAN controllers.
The tool uses the firmware file located in file
*/lib/firmware/mrw/mrw-firmware-m32.hex*. There is no need for any model file since tool collects the IDs of all CAN controllers using the PING command. After that it is expected that all controllers will answer to any request command.

## Program flow
1. Send a broadcast PING. All answering CAN controllers are registered by its ID.
2. Send a broadcast RESET. Now all CAN controllers are resetting and after that entering the bootloader mode. A fail is registered if not all controllers send a MSG_RESET_PENDING.
3. The bootloader itself does not send anything. So we have to try responsiveness by sending the FLASH_REQ command. If all controllers have
answered with a MSG_OK response all controllers are ready to flash. The bootloader accept FLASH_REQ commands for two seconds. After that the normal firmware is started.
4. The firmware is sent using the broadcast FLASH_DATA command. Note that this command never responds for performance reason. If the controllers would respond there would be the multiple of controller count traffic occuring on CAN bus. After sending a complete flash page there is a small time gap so the controllers can flash the sent data into memory.
5. After completing the firmware sending the checksum validation happens by sending the FLASH_CHECK command with the assumed checksum. If the checksum is OK the controllers respond with a MSG_OK and enter normal firmware execution resulting in RESET / MSG_BOOTED and GETVER / MSG_OK responses. If the checksum is not OK the bootloader remains active to give the chance of a flash retry. Note that this tool does not support this
feature.

This diagram shows the following sequence diagram:

```mermaid
sequenceDiagram
	Participant T as Tool
	Participant F as Firmware

	Note right of T: Collect all CAN nodes.
	T ->> +F: PING
	F ->> -T: PING: MSG_OK

	Note right of T: Activate bootloader.
	T ->> +F: RESET
	F ->>  T: PING: MSG_OK
	F ->> -T: PING: MSG_RESET_PENDING
	loop until MSG_OK
		T ->> +F: FLASH_REQ
	end
	F ->> -T: FLASH_REQ: MSG_OK

	Note left of F: Now bootloader is active and ready to flash.
	loop Flashing
		T ->> F: FLASH_DATA
	Note left of F: FLASH_DATA does not respond!
	end

	Note right of T: Flashing is complete now validate checksum.
	T ->> F: FLASH_CHECK
	activate F

	F ->> T: FLASH_CHECK: MSG_OK
	Note left of F: If response is MSG_OK the firmware will start directly.
	activate F

	F ->> T: GETVER: MSG_OK
	F ->> T: RESET: MSG_BOOTED
	deactivate F
	deactivate F
	Note left of F: New firmware has started.
```

The internal behaviour is controlled by the following statechart:

<img src="../statecharts/images/UpdateStatechart_0.png"/>
