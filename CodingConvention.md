# Coding conventions

## C++ Core Guidelines
In doubt always use the C++ Core Guideline:

https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines

## Documentation
The project uses Doxygen for API documentation. The documentation is placed into the header to automatically add the documentation to derived classes. This does not excuse anybody to document inside the implementation files, e.g. information about algorithms or technical functionality.

## Formatting

### astyle
Use astyle for beautifying and uniform appearance of the source code. To install astyle under Ubuntu use the command:

```
sudo apt-get install astyle
```

There exists an astylerc in this project so you may simply call ```make astyle``` to beautify the source code.

### Indentation
The identation are true 4 tabs. After the first non whitespace character use spaces instead.

## Blocks and scopes

### One lined expressions
One lined expressions are always surrounded with curly braces to prevent different behaviour on extending the expression.

### Curly braces for switch-case:
Use curly braces in a case only if you need local variables. The final break is outside the braces.

Example:
```
switch(enum_var)
{
case ENUM:
	{
		// Something with local variables.
		// break not here
	}
	break; // but here
}
```

## Naming
You can find general information about naming in Wikipedia. The ungarian notation is never used!

https://en.wikipedia.org/wiki/Hungarian_notation

### Class and enumeration names
Classes and enumerations are named in upper camel case.

Example: CodingStandard

### Methods
Methods are named in lower camel case.

Example: lowerCamelCase()

### Null pointer
Null pointer checks are always against the ```nullptr``` keyword.

### Getter und Setter
The getter does not need a preceeding "get" but the setter uses "set" which conforms the Qt rule.

Example: ```code()``` for the getter and ```setCode()``` for the setter.

### Member variables and local variables
Member variablen are completely lower case and the seperator is the under score.

Example: member_variable.

### Enumerations and constants
Enumerations and constants are completely upper case and the seperator is the under score.

Example: ENUM_ELEMENT.

## Sorting visibility sections
The internal class visibility is as follows.
1. public
2. protected
3. private

It may be allowed to put member variables in the starting default private location of a class.

## Namespaces
Never use ```using namespace``` inside headers to prevent ambiguity. See https://stackoverflow.com/questions/19940081/using-keyword-in-header-implementation-in-c for more information.

## Sorting includes
Project specific includes start wird double quotes (```#include "test.h"```). All other use brackets (```#include <test.h>```). The sort order  is by dependency inside one block. These block are sorted as follows:
1. C/C++ includes
2. Qt includes
3. MRW package includes
4. project includes.

But generally all header have to work even if not in the given order.

## SPDX header
Every source file should start with a SPDX definition:

```
#
#  SPDX-License-Identifier: MIT
#  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
#
```
Replace the author on your own demands.

## Include guards
Use
```
#pragma once
```

 as include guard for compiler opimization and additionally ```#idndef/#define/endif``` for compatibility reasons.

The ```#pragma once``` should be the first command just after the copyright.

References:
- https://en.wikipedia.org/wiki/Include_guard
- https://en.wikipedia.org/wiki/Pragma_once

The name of the header should be built from the full qualified namespace and class name.

## Compiler options
Always activate warnings! Always think that each warning may be a bug!

Activate the following options:
- -Wall
- -Wextra
- -Wshadow
- -Wsuggest-override
- -fstack-protector-strong
