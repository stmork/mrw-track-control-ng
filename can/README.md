# The mrw::can namespace

The mrw::can namespace provides the CAN bus infrastructure using the Qt serialbus API. The classes are prepared for using with the model railway infrastructure. So all model railway constants for Command and Result codes are defined in this namespace. To use the CAN bus simply deriv a class from the MrwBus Service class and overload the MrwBusService::process() method. This method gets the preparsed CAN message as a MrwMessage instance.

For the CAN bus communication you can configure the interface and plugin in the QCanBusDevice class manner. To use a unique behaviour on a per host basis use the CanSettings class

The MrwBusService allows to connect to the CAN bus manually. So you have to set the auto_connect parameter to false and call the QCanBusService::connectDevice() method manually.
