//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2023 Steffen A. Mork
//

pipeline
{
	agent
	{
		label 'QT'
	}

	stages
	{
		stage ('Build')
		{
			steps
			{
				sh """
				rm -rf doc lcov-out */moc_* */ui_* */*.o
				test -f Makefile && make clean distclean
				qmake -r CONFIG+=gcov
				make -j
				"""
			}
		}

		stage ('Doxygen')
		{
			steps
			{
				sh 'make doxygen'
				publishHTML([
					allowMissing: false,
					alwaysLinkToLastBuild: false,
					keepAll: false,
					reportDir: 'doc/html',
					reportFiles: 'index.html',
					reportName: 'MRW-NG Doxygen',
					reportTitles: ''])
			}
		}

		stage ('CppCheck')
		{
			steps
			{
				sh 'make cppcheck'
				publishCppcheck pattern: 'cppcheck.xml'
			}
		}

		stage ('Test')
		{
			steps
			{
				sh 'make sct-unit valgrind'
//				xunit checksName: '', tools: [
//					QtTest(excludesPattern: '', pattern: 'qtest-*.xml', stopProcessingIfError: false),
//					GoogleTest(excludesPattern: '', pattern: 'statecharts/test/gtest-*.xml', stopProcessingIfError: false),
//					Valgrind(excludesPattern: '', pattern: 'valgrind*.xml', stopProcessingIfError: false)]

				step([$class: 'XUnitPublisher',
					checksName: 'Unit Tests',
					testTimeMargin: '3000',
					thresholdMode: 0,
					thresholds: [], tools: [
						GoogleTest(excludesPattern: '', pattern: 'statecharts/test/gtest-*.xml'),
						QtTest(excludesPattern: 'qtest-*.xml', pattern: ''),
						Valgrind(excludesPattern: '', pattern: 'valgrind*.xml')]])

			}
		}

		stage ('Coverage')
		{
			steps
			{
				sh 'make lcov'
				publishHTML([
					allowMissing: false,
					alwaysLinkToLastBuild: false,
					keepAll: false,
					reportDir: 'lcov-out',
					reportFiles: 'index.html',
					reportName: 'MRW-NG Coverage Report',
					reportTitles: ''])
			}
		}
	}

	post
	{
		always
		{
			chuckNorris()
			step([$class: 'Mailer', recipients: 'info@eisenbahnsteuerung.org'])
		}
	}
}
