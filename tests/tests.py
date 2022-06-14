from sys import stdout
import pytest
import os
import subprocess as sp
from test_classes.classes import UnitTest 

@pytest.mark.skip(reason='No tests implemented')
def test_cases():
    pass


@pytest.mark.parametrize('executable_name', [ 
    'test_project',
])
def test_unit(executable_name):
    test = UnitTest(executable_name)
    passed = test.run()
    assert passed