import os
import platform
from pathlib import Path
import pytest
from ..test_classes.classes import UnitTest 

def get_unit_tests():
    is_win = platform.system() == "Windows"
    unit_tests_path = Path(os.environ['LIGHTNING_TESTS']) / 'unit_tests'
    cpp_files = list(unit_tests_path.glob('test_*.cpp'))
    executables = [file.stem + ('.exe' if is_win else '') for file in cpp_files]
    return executables

@pytest.mark.parametrize('executable_name', get_unit_tests())
def test_run_unit_tests(executable_name):
    t = UnitTest(f"{os.environ['LIGHTNING_BUILD_DIR']}/tests/unit_tests/{executable_name}")
    assert t.run()
