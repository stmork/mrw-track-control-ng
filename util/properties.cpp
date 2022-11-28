//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2022 Steffen A. Mork
//

#include <QFile>
#include <QTextStream>

#include "properties.h"

using namespace mrw::util;
/**
 * Regular expression prolog:
 * \d matches one digit.
 * \s whitespace character
 * \w matches one digit or case insensitive letter.
 * "." matches one character.
 * \d+ matches a number with at least one digit
 * (\d+) marks a matching group
 *
 * Since we use C++11 raw string literals we do not need to escape the '\' char to "\\".
 *
 * There is a nice regular expression test site where you can build a regular expression and test it agai
 * multiple test patterns: https://regex101.com/
 *
 * The following test patterns may be used:
 * @verbatim
# Only a comment
 # another comment
AvalidProp=xyvc_,
 Another11Prop = ccc
Special=a.-_,
11ss=xx
1=2
ss1=2
empty=
@endverbatim
 */
const std::regex  Properties::regex(R"(^\s*([a-zA-Z]\w+)\s*=\s*([\w_\-\.\,]*))");
const std::string Properties::EMPTY = "";

Properties::Properties(const QString & filename)
{
	read(filename);
}

bool Properties::read(const QString & filename)
{
	QFile file(filename);
	const bool success = file.open(QIODevice::ReadOnly);

	if (success)
	{
		QTextStream in(&file);

		in.setCodec("ISO 8859-15");
		while (!in.atEnd())
		{
			std::string line = in.readLine().toStdString();
			std::smatch matcher;

			if (std::regex_match(line, matcher, regex))
			{
				Q_ASSERT(matcher.size() == 3);

				const std::string & key   = matcher[1];
				const std::string & value = matcher[2];

				this->emplace(key, value);
			}
		}
		file.close();
	}
	return success;
}

const std::string & mrw::util::Properties::lookup(const std::string & key) const
{
	return find(key) != end() ? at(key) : EMPTY;
}
