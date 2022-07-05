from ..test_classes.classes import CaseTest
import pytest
import os
from pathlib import Path
@pytest.mark.parametrize('input_file,output_file',[
    pytest.param('2_positive_1_negative.light', 'electric_field.png'),
    pytest.param('1_positive_1_negative.light', 'electric_field.png'),
    pytest.param('2_positive.light', 'electric_field.png'),
])
def test_electric_potential_field(input_file, output_file, tmpdir, image_regression):
    test_dir = f'{os.getenv("LIGHTNING_TESTS")}/case_tests/inputs'
    output_path = Path(tmpdir / output_file)
    runner = CaseTest(f'{test_dir}/{input_file}', str(output_path))
    runner.run()
    image_regression.check(output_path.read_bytes(), expect_equal=True)
    