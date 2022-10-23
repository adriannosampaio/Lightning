import os
import subprocess as sp


test_dir = f'{os.getenv("LIGHTNING_TESTS")}/case_tests/inputs'

def params_to_command(charges_file, M, S, N, R, D, I, noise):    
    #-f tests/case_tests/inputs/2_positive_1_negative.light -M 50 -S 15 -N 4 -R 29 -D 40 -I 1400 --noise .7
    return f"-f {test_dir}/{charges_file} -M {M} -S {S} -N {N} -R {R} -D {D} -I {I} --noise {noise}".split()


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
	def __init__(self, arguments, output_file) -> None:
		self.executable = f'{os.getenv("LIGHTNING_BIN_DIR")}/zeus'
		self.arguments = arguments
		self.output_file = output_file

	def run(self):
		output = ''
		exit_code = 0
		try:
			output = sp.check_output([
				self.executable, 
				*params_to_command(*self.arguments),
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