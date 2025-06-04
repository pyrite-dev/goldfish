pipeline {
	agent any
	environment {
		WEBHOOK_NISHBOX = credentials("webhook-nishbox")
		WEBHOOK_ORIN = credentials("webhook-orin")
		WEBHOOK_PRIVATE1 = credentials("webhook-private1")
	}
	stages {
		stage("Get submodules") {
			steps {
				sh "git submodule update --init --recursive --force"
			}
			post {
				always {
					post_always(true, false)
				}
			}
		}
		stage("Deploy document") {
			steps {
				sh "doxygen"
				sh "rm -rf /var/www/pyrite.goldfish-doc/*"
				sh "cp -rf doc/html/* /var/www/pyrite.goldfish-doc/"
			}
			post {
				always {
					post_always(true, true, true)
				}
			}
		}
	}
}

def post_always(cmt,art,alw=false){
	def list = [env.WEBHOOK_NISHBOX, env.WEBHOOK_ORIN, env.WEBHOOK_PRIVATE1]
	for(int i = 0; i < list.size(); i++){
		if(alw || currentBuild.currentResult != "SUCCESS"){
			discordSend(
				webhookURL: list[i],
				link: env.BUILD_URL,
				result: currentBuild.currentResult,
				title: "${env.JOB_NAME} - ${env.STAGE_NAME}",
				showChangeset: cmt,
				enableArtifactsList: art,
				description: "**Build:** ${env.BUILD_NUMBER}\n**Status:** ${currentBuild.currentResult}"
			)
		}
	}
}
