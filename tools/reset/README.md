# The MRW-Reset tool
The MRW-Reset simply restarts all connected CAN controllers by sending a broadcast RESET command.

## Program flow
1. Send a broadcast RESET. Now all CAN controllers are resetting and after that entering the bootloader mode. Ech CAN controller respond twice by sending a RESET / MSG_OK and a RESET / MSG_RESET_PENDING response. These answeres are registered.
2. The bootloader itself does not send anything. The bootloader accept FLASH_REQ commands for two seconds. If no FLASG_REQ command was sent the normal firmware starts.
3. Since there was no request for flashing the normal firmware start execution resulting in RESET / MSG_BOOTED and GETVER / MSG_OK responds. If all CAN controllers responded the tool stops execution.

## LED states

During a boot cycle with or without flashing a new firmware the LEDs indicates the internal operation state:

green|yellow|remakrs
----|----|---------
blinking|off|Bootloader waits for flash request.
off|cont.|Bootloader is flashing new firmware.
cont.|blinking|Bootloader waits for reset, no firmware flashed, yet.
off|off|Firmware waits for reset.
cont.|off|Firmware executing.
cont.|cont.|Firmware executing, CAN error occured.

## Sequence diagram
This diagram shows the following sequence diagram:

```mermaid
sequenceDiagram
	Participant T as MRW-Reset
	Participant F as Firmware

	rect rgb(240,255,240)
		F --> LED: green up

		Note right of T: Request reset.
		T ->> +F: RESET
		F ->>  T: RESET: MSG_OK
		F ->>  T: RESET: MSG_RESET_PENDING
		deactivate F
	end

	Note right of T: After about one second the bootloader is active.

	rect rgb(255,240,240)
		F ->> LED: blinking green

		Note right of T: After two seconds without FLASH_REQ command the firmware starts.
	end

	rect rgb(240,255,240)
		activate F
		F ->> LED: all off


		F ->> LED: green up
		F ->> T: GETVER: MSG_OK
		F ->> T: RESET: MSG_BOOTED

		deactivate F
		Note left of F: Firmware has restarted.
	end
```
