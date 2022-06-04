from sys import stdout
import pytest
import os
import subprocess as sp

@pytest.mark.skip(reason='No tests implemented')
def test_cases():
    pass


@pytest.mark.parametrize('executable', [ 
    'test_project',
])
def test_unit(executable):
    base_dir = f'{os.getenv("LIGHTNING_BUILD_DIR")}/install/unit_tests'
    output = ''
    exit_code = 0
    try:
        output = sp.check_output([f'{base_dir}/{executable}'], shell=True)
    except sp.CalledProcessError as e:                                                                                                   
        exit_code = e.returncode
        output = e.output

    if output:
        output = output.decode("utf-8")
        print(f'Output:\n{output}')
    assert exit_code == 0