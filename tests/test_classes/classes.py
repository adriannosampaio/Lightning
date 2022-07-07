import os
import subprocess as sp


class Test():
	def __init__(self):
		pass
	def run(self):
		return True

class UnitTest(Test):

	
	def __init__(self, executable) -> None:
		self.executable = executable

	def run(self):
		output = ''
		exit_code = 0
		try:
			output = sp.check_output([f'{self.executable}'], shell=True)
		except sp.CalledProcessError as e:                                                                                             
			exit_code = e.returncode
			output = e.output
		
		if output:
			output = output.decode("utf-8")
			print(f'Output:\n{output}')
		return exit_code == 0

class CaseTest(Test):

	
	def __init__(self, input_file, output_file) -> None:
		self.executable = f'{os.getenv("LIGHTNING_BIN_DIR")}/Lightning'
		self.input_file = input_file
		self.output_file = output_file

	def run(self):
		output = ''
		exit_code = 0
		try:
			output = sp.check_output([
				self.executable, 
				'-f', 
				self.input_file,
				'-o', 
				self.output_file
			])
		except sp.CalledProcessError as e:                                                                                             
			exit_code = e.returncode
			output = e.output
		
		if output:
			output = output.decode("utf-8")
			print(f'Output:\n{output}')
		return exit_code == 0