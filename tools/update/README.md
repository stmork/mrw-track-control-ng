# The MRW-Update tool
The <code>MRW-Update</code> tool updates the firmware of all connected CAN controllers. The tool uses the firmware file located in file */lib/firmware/mrw/mrw-firmware-m32.hex*. There is no need for any model file since the tool collects the IDs of all CAN controllers using the PING command. After that it is expected that all controllers will answer to any request command.

## Program flow
1. Send a broadcast RESET. Now all CAN controllers are resetting and after that entering the bootloader mode.
2. Send a broadcast PING after 1250 ms. All answering CAN controllers are registered by its ID. The bootloader itself does only respond with messages to PING, FLASH_REQ and FLASH_CHECK. So we have to try responsiveness by sending first the PING command and later the FLASH_REQ command.
3. 250 ms later it was enough time to respond with a MSG_OK response and all controllers are ready to flash. The bootloader accept FLASH_REQ commands for two seconds. Since 500 ms are over there are 1500 ms remaining to initiate flashing. Without sending a FLASH_REQ command the bootloader starts he normal firmware if available. If no firmware was flashed yet the bootloader only responds to a RESET command indicating a green LED and yellow blinking LED.
4. After initiating flashing using the FLASH_REQ command the new firmware is sent using the broadcast FLASH_DATA command. Note that this command never responds for performance reason. If the controllers would respond there would be the multiple of controller count traffic occuring on CAN bus. After sending a complete flash page there is a small time gap so the controllers can flash the sent data into flash memory.
5. After completing the firmware sending the checksum validation happens by sending the FLASH_CHECK command with the assumed checksum. If the checksum is OK the controllers respond with a MSG_OK and enter normal firmware execution resulting in RESET / MSG_BOOTED and GETVER / MSG_OK responses. If the checksum is not OK the bootloader remains active to give the chance of a flash retry. Note that this tool does not support this
feature.

## LED states

During a boot cycle with or without flashing a new firmware the LEDs indicates the internal operation state:

green|yellow|remarks
----|----|---------
blinking|off|Bootloader waits for flash request.
off|cont.|Bootloader is flashing new firmware.
cont.|blinking|Bootloader waits for reset, no firmware flashed, yet.
off|off|Firmware waits for reset.
cont.|off|Firmware executing.
cont.|cont.|Firmware executing, CAN error occured.

## Sequence diagram
This diagram shows the following sequence diagram. The green areas are the firmware runtime and the red area is the bootloader runtime.

```mermaid
sequenceDiagram
	Participant T as MRW-Update
	Participant F as Firmware

	rect rgb(240,255,240)
		F --> LED: green up

		Note right of T: Activate bootloader.
		T ->> +F: RESET
		F ->>  T: RESET: MSG_OK
		F ->> -T: RESET: MSG_RESET_PENDING
	end

	rect rgb(255,240,240)
		F ->> LED: blinking green
		Note left of F: Now bootloader is active.
		Note right of T: Collect all CAN nodes.
		T ->> +F: PING
		F ->> -T: PING: MSG_OK

		loop until MSG_OK
			T ->> +F: FLASH_REQ
		end
		F ->> LED: green off, yellow up
		F ->> -T: FLASH_REQ: MSG_OK

		Note left of F: Now bootloader ready to flash.
		loop Flashing
			T ->> F: FLASH_DATA
		Note left of F: FLASH_DATA does not respond!
		end

		Note right of T: Flashing is complete now validate checksum.
		T ->> +F: FLASH_CHECK

		F ->> -T: FLASH_CHECK: MSG_OK
		Note left of F: If response is MSG_OK the firmware will start  directly.
	end

	rect rgb(240,255,240)
		activate F
		F ->> LED: all off

		F ->> LED: green up
		activate F

		F ->> T: GETVER: MSG_OK
		F ->> T: RESET: MSG_BOOTED
		deactivate F
		deactivate F
		Note left of F: New firmware has started.
	end
```

## Statechart
The flashing behaviour of the <code>MRW-Update</code> tool is controlled by the following statechart:

<img src="../../statecharts/images/UpdateStatechart_0.png"/>
