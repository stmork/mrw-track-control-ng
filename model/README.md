# The mrw::model namespace

The mrw::model namespace contains all data/model classes for describing a complete model railway. It describes all model elements such as rails, switches and signals as well as the micro controller infrastructure. Additionally it describes how the model railway elements are connected to the micro controller infrastructure.

The main tasks of these namespace classes are to read the description from a modelrailway file which is in fact a XMI/XML file and then to hold the state during live time.

The root element of the model/data is the ModelRailway class. All sub classes are more or less accessible in a direct manner. A main sub tree are the Controller classes which describe the micro controller infrastructure. The Region class contains all model railway elements. The first layer under the Region class is the Section class which contain all AssemblyPart elements such as RailPart or Signal elements.

In detail the class hierarchy is more complex to describe the behaviour of each element.

Some classes are part of the MVC paradigma to to show the state onto the track control screen. See the mrw::ctrl and mrw::ui namespaces for more information.

