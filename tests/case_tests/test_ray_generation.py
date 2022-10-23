from ..test_classes.classes import CaseTest
import pytest
import os
from pathlib import Path
import numpy as np



def generate_test_parameters(input_file):
    number_of_preliminary_candidates = [10*x for x in range(1, 8, 3)]
    number_of_candidates = [x for x in range(1, 5)]
    segment_sizes = [10*x for x in range(1, 4, 2)]
    noises = np.linspace(0.0, 0.4, num=3)
    arguments_list = []
    for noise in noises:    
        for m in number_of_preliminary_candidates:
            for n in number_of_candidates:
                for seg in segment_sizes:
                    for elimination_radius in range(0, seg, 5):
                        arguments_list.append(
                            pytest.param(
                                input_file,
                                m,
                                seg,
                                n,
                                elimination_radius,
                                2*seg,
                                1000,
                                noise,
                                f'field_result_{input_file}.png'
                            )
                        )
    print(f'Generated {len(arguments_list)} tests...')
    return arguments_list
                            

@pytest.mark.parametrize('charges_file, M, S, N, R, D, I, noise,output_file',[
    *generate_test_parameters('2_positive_1_negative.light')
])
def test_electric_potential_field(charges_file, M, S, N, R, D, I, noise, output_file, tmp_path, image_regression):
    output_path = Path(tmp_path / output_file)
    runner = CaseTest([charges_file, M, S, N, R, D, I, noise], str(output_path))
    runner.run()
    image_regression.check(output_path.read_bytes(), expect_equal=True)
    