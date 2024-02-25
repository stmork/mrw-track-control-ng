//
//  SPDX-License-Identifier: MIT
//  SPDX-FileCopyrightText: Copyright (C) 2008-2024 Steffen A. Mork
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
				rm -rf doc lcov-out cccc */moc_* */ui_* */*.o
				test -f Makefile && make clean distclean
				qmake -r CONFIG+=gcov
				make -j `nproc`
				"""
			}
		}

		stage ('Doxygen')
		{
			steps
			{
				sh 'make doxygen'
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

		stage ('CCCC')
		{
			steps
			{
				sh 'make cccc'
			}
		}

		stage ('Test')
		{
			steps
			{
				sh 'make sct-unit valgrind'
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

			publishHTML([
				allowMissing: false,
				alwaysLinkToLastBuild: false,
				keepAll: false,
				reportDir: 'api-doc/html',
				reportFiles: 'index.html',
				reportName: 'MRW-NG Doxygen',
				reportTitles: ''])

			publishHTML([
				allowMissing: false,
				alwaysLinkToLastBuild: false,
				keepAll: false,
				reportDir: 'cccc',
				reportFiles: 'cccc.html',
				reportName: 'MRW-NG Code Complexity',
				reportTitles: ''])

			xunit ([
				QtTest(    pattern: 'qtest-*.xml', stopProcessingIfError: false),
				GoogleTest(pattern: 'statecharts/test/gtest-*.xml', stopProcessingIfError: false)])
			xunit ([
				Valgrind(  pattern: 'valgrind*.xml', stopProcessingIfError: false)])
		}
	}
}
