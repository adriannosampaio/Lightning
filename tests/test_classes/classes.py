import os
import subprocess as sp


class Test():
	def __init__(self):
		pass
	def run(self):
		return True

class UnitTest(Test):

	base_dir = f'{os.getenv("LIGHTNING_BUILD_DIR")}/install/unit_tests'
	
	def __init__(self, executable) -> None:
		self.executable = executable

	def run(self):
		output = ''
		exit_code = 0
		try:
			output = sp.check_output([f'{UnitTest.base_dir}/{self.executable}'], shell=True)
		except sp.CalledProcessError as e:                                                                                             
			exit_code = e.returncode
			output = e.output
		
		if output:
			output = output.decode("utf-8")
			print(f'Output:\n{output}')
		return exit_code == 0