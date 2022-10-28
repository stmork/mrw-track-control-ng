pipeline
{
	agent any

	stages
	{
		stage ('Build')
		{
			steps
			{
				sh """
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
				sh 'doxygen'
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
				sh 'valgrind --xml=yes --xml-file=valgrind.xml test/MRW-Test'
				xunit checksName: '', tools: [
					QtTest(excludesPattern: '', pattern: 'qtest-*.xml', stopProcessingIfError: true),
					Valgrind(excludesPattern: '', pattern: 'valgrind*.xml', stopProcessingIfError: false)]		
			}
		}
	}

	post
	{
		always
		{
			chuckNorris()
			step([$class: 'Mailer', recipients: 'linux-dev@morknet.de'])
		}
	}
}
