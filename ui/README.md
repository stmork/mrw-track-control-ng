# The mrw::ui namespace

The mrw::ui namespace contains all custom widget classes needed for the MRW next generation track control. All widgets are derived from the BaseWidget class. Special classes like the BarWidget or OpModeWidget are directly derived.

All classes derived from ControllerWidget are view classes in the meaning of the MVC paradigma (see: https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller). The model contains simply the data. The view represents simply this data. Both don't talk directly but using an intermediate controller which contains a sort of business logic. Generally speaking the "view" BaseWidget does not see any model class.

The model may be any class from the mrw::model namespace, The view classes are in the mrw::ui namespace and the controller classes are in this mrw::ctrl namespace.

```mermaid
classDiagram

class ModelClass
<<Model>> ModelClass

class BaseController
<<Controller>> BaseController
QObject <|-- BaseController

class ControllerWidget
<<View>> ControllerWidget

QWidget <|-- BaseWidget
BaseWidget <|-- ControllerWidget
ControllerWidget ..> BaseController : uses
BaseController ..> ControllerWidget : signals
BaseController o.. ModelClass : uses

```

Actually there are four MVC groups:
1. Rail - RailController - RailWidget
2. RegularSwitch - RegularSwitchController - RegularSwitchWidget
3. DoubleCrossSwitch - DoubleCrossSwitchController - DoubleCrossSwitchWidget
4. Signal - SignalController - SignalWidget

The MVC for signals is a little bit different because a controller may represent a group of signals like a combined main and distant signal.

## RailWidget

As an example this scenario shows the MVC for a rail.

```mermaid
classDiagram

class BaseController
<<Abstract>> BaseController

class RailController
<<Interface>> RailController

class RailControllerProxy
<<Controller>> RailControllerProxy

class BaseWidget
<<abstract>> BaseWidget

class ControllerWidget
<<abstract>> ControllerWidget

class RailWidget
<<View>> RailWidget

class Rail
<<Model>> Rail

QObject <|-- BaseController
BaseController <|-- RailController
BaseWidget <|-- ControllerWidget
ControllerWidget <|-- RailWidget

QWidget <|-- BaseWidget
RailWidget ..> RailController : uses
RailControllerProxy o.. Rail : uses
RailControllerProxy ..> RailWidget : signals

RailController <|-- RailControllerMock
RailController <|-- RailControllerProxy

```

## RegularSwitchWidget

The regular switch widget works like the widget for the double cross widget. So we describe only the regulur switch architecture. In opposite to a simple rail a switch acts as a device. So the controller proxy class inherits the ControllerRegistrand class which registers the controller to a central registry - the ControllerRegistry service. The MrwBusService uses the ControllerRegistry to map an incoming CAN message containing CAN node id and device unit number to the corresponding Controller which handles the CAN message.

```mermaid
classDiagram

class BaseController
<<Abstract>> BaseController

class SwitchController
<<Interface>> SwitchController

class ControllerRegistry
<<Service>> ControllerRegistry

class MrwMessageDispatcher
<<Service>> MrwMessageDispatcher

class ControllerRegistrand
<<Interface>> ControllerRegistrand

class SwitchControllerProxy
<<Controller>> SwitchControllerProxy

class BaseWidget
<<abstract>> BaseWidget

class ControllerWidget
<<abstract>> ControllerWidget

class SwitchWidget
<<View>> SwitchWidget

class Switch
<<Model>> Switch

QObject <|-- BaseController
BaseController <|-- SwitchController

QWidget <|-- BaseWidget
BaseWidget <|-- ControllerWidget
ControllerWidget <|-- SwitchWidget

QObject <|-- MrwBusService
MrwBusService o-- QCanBus
MrwBusService *-- QCanBusDevice
MrwBusService <|-- MrwMessageDispatcher

MrwMessageDispatcher ..> ControllerRegistry : uses
MrwMessageDispatcher ..> SwitchControllerProxy : messages

SwitchWidget ..> SwitchController : uses
SwitchControllerProxy o.. Switch : uses
SwitchControllerProxy ..> SwitchWidget : signals
ControllerRegistrand ..> ControllerRegistry : register

SwitchController <|-- SwitchControllerMock
SwitchController <|-- SwitchControllerProxy
ControllerRegistrand <|-- SwitchControllerProxy

```
