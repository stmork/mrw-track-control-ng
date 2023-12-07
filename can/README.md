# The mrw::can namespace

To use CAN bus via *socketcan* you need a configured CAN bus adapter. By default the corresponding CAN bus driver is loaded on Linux boot. Look with ```ifconfig -a``` if the *can0* interface is generally available. Then you can configure the CAN bus by calling the following commands:
```shell
ifconfig can0 type can bitrate 125000
ifconfig can0 txqueuelen 1000
ip link set up can0
```
**Note:** The CAN bus bitrate must be 125.000 bits per second.

After that you can verify the working CAN bus by calling:
```shell
mrw-host(~) 4> ip -details link show can0
3: can0: <NOARP,UP,LOWER_UP,ECHO> mtu 16 qdisc pfifo_fast state UP mode DEFAULT group default qlen 1000
    link/can  promiscuity 0 minmtu 0 maxmtu 0
    can state ERROR-WARNING restart-ms 0
          bitrate 125000 sample-point 0.875
          tq 500 prop-seg 6 phase-seg1 7 phase-seg2 2 sjw 1
          gs_usb: tseg1 1..16 tseg2 1..8 sjw 1..4 brp 1..1024 brp-inc 1
```

To shutdown the CAN bus you can call:
```shell
ip link set down can0
```

## Software support

The mrw::can namespace provides the CAN bus infrastructure using the Qt serialbus API. The classes are prepared for using with the model railway infrastructure. So all model railway constants for Command and Result codes are defined in this namespace. To use the CAN bus simply deriv a class from the MrwBus Service class and overload the MrwBusService::process() method. This method gets the preparsed CAN message as a MrwMessage instance.

For the CAN bus communication you can configure the interface and plugin in the QCanBusDevice class manner. To use a unique behaviour on a per host basis use the CanSettings class

The MrwBusService allows to connect to the CAN bus manually. So you have to set the auto_connect parameter to false and call the QCanBusService::connectDevice() method manually.
