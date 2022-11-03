from invoke import task
import os
from pathlib import Path
import platform


platform = platform.system()
build_path = Path(os.getenv("LIGHTNING_BUILD_DIR")).as_posix()
root_path = Path(os.getenv("LIGHTNING_ROOT")).as_posix()

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
def clean(c):
    print("Cleaning!")
    with c.cd(root_path):
        c.run(f"rm -rf build/*")

last_build_mode="Debug"
@task
def configure(c, mode="Debug"):
    print(str(f'"{build_path}"'))
    os.makedirs(build_path, exist_ok=True)
    with c.cd(str(build_path)):
        c.run('mkdir -p install')
        last_build_mode = mode
        c.run(f"cmake .. -DCMAKE_BUILD_TYPE={mode} -DCMAKE_INSTALL_PREFIX=install -DCMAKE_EXPORT_COMPILE_COMMANDS=ON")

@task(pre=[clean, configure])
def build(c, mode=last_build_mode):
    with c.cd(str(build_path)): 
        print("Building!")
        if platform == 'Windows':
            c.run(f"cmake --build . --target install --config {mode}")
        else:
            c.run(f"make && make install")


@task
def test(c, pattern=None, verbose=False, force_regen=False, num_threads="auto"):
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
                '--basetemp=tmp',
		        f'{"--force-regen" if force_regen else ""}',
                f'-n {num_threads}'
            ]), 
            pty=platform=="Linux"
        )
