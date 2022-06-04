from invoke import task
import os
from pathlib import Path

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
def configure(c, mode="Debug"):
    print(str(f'"{build_path}"'))
    os.makedirs(build_path, exist_ok=True)
    with c.cd(str(build_path)):
        c.run('mkdir -p install')
        generator = f'-G Ninja'
        c.run(f"cmake .. -DCMAKE_BUILD_TYPE={mode} -DCMAKE_INSTALL_PREFIX=install {generator}")

@task(pre=[configure])
def build(c):
    print("Building!")
    with c.cd(str(build_path)):
        c.run(f"ninja -j8 && ninja install")

@task
def clean(c):
    print("Cleaning!")
    with c.cd(root_path):
        c.run(f"rm -rf build/*")

@task
def test(c, verbose=True):
    print("Running tests!")
    with c.cd(f'{str(root_path)}/tests'):
        c.run('ls')
        c.run('pytest tests.py', pty=True)
