# MRW Track Control NG
This is the next generation Qt based track control software based on the MRW
project. It reads the modelrailway file, which is XML/XMI based and displays
the GUI to control the model railway.

## Data flow archtecture

```mermaid
flowchart TB
	subgraph Eclipse
	Modelrailway.umlclass -- converts --> Ecore --> GenModel
	GenModel --> Modelleditor-Plugin
	Ecore --> Modelleditor-Plugin
	end
	Modelleditor-Plugin -- edit --> MRW.modelrailway -- uses --> MRW-NG
	subgraph Qt-App
	MRW-NG -. creates .-> Settings
	Signal.properties -. converts .-> MRW-NG
	Gruppe.properties -. converts .-> MRW-NG
	Gleisteile.properties -. converts .-> MRW-NG
	end
```
