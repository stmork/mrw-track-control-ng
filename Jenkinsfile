pipeline
{
	agent any

	stages
	{
		stage('Build')
		{
			steps
			{
				sh """
				test -f Makefile && make clean distclean
				qmake -r
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

		stage('CppCheck')
		{
			steps
			{
				sh 'make cppcheck'
				publishCppcheck pattern: 'cppcheck.xml'
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
