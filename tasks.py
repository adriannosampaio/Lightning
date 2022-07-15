from invoke import task
import os
from pathlib import Path
import platform

build_path = Path(os.getenv("LIGHTNING_BUILD_DIR")).as_posix()
root_path = Path(os.getenv("LIGHTNING_ROOT")).as_posix()
is_linux = platform.system() == 'Linux'

@task
def project_setup(c):
    with c.cd(str(root_path)):
        c.run('pre-commit install')
        c.run('git submodule update')

@task
def check(c):
    with c.cd(str(root_path)):
        c.run('cppcheck **/ --enable=all --inline-suppr --suppress=missingIncludeSystem')

@task
def doxygen(c):
    with c.cd(root_path):
        c.run('doxygen docs/doxyfile')
        c.run('sensible-browser docs/html/index.html')


@task
def configure(c, mode="Debug", install_dir=''):
    print(str(f'"{build_path}"'))
    os.makedirs(build_path, exist_ok=True)
    with c.cd(str(build_path)):
        install_flag = f'-DCMAKE_INSTALL_PREFIX={install_dir}' if install_dir else ''
        if is_linux:
            c.run('mkdir -p install')
        else:
            c.run('if not exist "install" mkdir install')
        generator = f'-G Ninja' if is_linux else ''
            #generator = 'Visual Studio 17 2022'
        c.run(f"cmake .. -DCMAKE_BUILD_TYPE={mode} {install_flag} {generator} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON")

@task(pre=[configure])
def build(c):
    with c.cd(str(build_path)): 
        if is_linux:
            print("Building!")
            c.run(f"ninja -j0 && ninja install")
        else:
            print("Windows platforms use Visual Studio for building!")
            print("Opening!")
            c.run('start Lightning.sln')

@task
def clean(c):
    print("Cleaning!")
    with c.cd(root_path):
        c.run(f"rm -rf build/*")

@task
def test(c, pattern=None, verbose=False):
    ''' Run all tests in the suite. It also allows for custom
        test selection and verbosity configuration

        @param pattern Specify a name pattern for the tests 
            to be executed

        @param verbose set pytest maximum verbosity value 
    '''
    print("Running tests!")
    with c.cd(f'{str(root_path)}/tests'):
        c.run(
            ' '.join([
                'pytest',
                f'{"-s" if verbose else ""} {"-k" if pattern is not None else ""}',
                f'{pattern or ""}',
                '--basetemp=tmp'
            ]), 
            pty=True
        )
