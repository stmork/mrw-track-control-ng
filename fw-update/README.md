# The MRW-Update tool
The MRW-Update tool updates the firmware of all connected CAN controllers.
The tool uses the firmware file located in
*/lib/firmware/mrw/mrw-firmware-m32.hex*. There is no need for any model file
since tool collects the IDs of all CAN controllers using the PING command.
After that it is expected that all controllers will answer to any request
command.

In detail the commands occur as follows:

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
