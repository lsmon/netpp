import os
import platform
import subprocess
import sys
import zipfile
import shutil

root_path = ""

def run_command(command):
    result = subprocess.run(command, shell=True)
    if result.returncode != 0:
        raise Exception(f"Command '{command}' failed with exit code {result.returncode}.")


def clone_logpp():
    print("Cloning logpp...")
    os.chdir(root_path)
    # Clone or update nlohmann/json using Git (you need Git installed)
    if not os.path.exists("logpp"):
        run_command("git clone https://github.com/lsmon/logpp.git logpp")
    else:
        os.chdir("logpp")
        run_command("git pull origin main")
        os.chdir("..")
    print("Cloning completed.")


def build_logpp():
    print("Building logpp...")

    logpp_path = os.path.join(root_path, "logpp")
    logpp_build = os.path.join(logpp_path, "cmake-build-debug")
    os.chdir(logpp_path)
    os.makedirs(logpp_build, exist_ok=True)

    os_name = platform.system()
    print(os_name)
    run_command("cmake -S " + logpp_path + " -B " + logpp_build)
    run_command("cmake --build " + logpp_build + " -j 14")
    print("Building completed.")


def cpack_logpp():
    print("Packing logpp")
    logpp_path = os.path.join(root_path, "logpp")
    logpp_build = os.path.join(logpp_path, "cmake-build-debug")
    os.chdir(logpp_build)
    run_command("cpack -C Debug")


def install_logpp():
    logpp_path = os.path.join(root_path, "logpp")
    logpp_build = os.path.join(logpp_path, "cmake-build-debug")
    logpp_libzip = os.path.join(logpp_build, "lib_logpp-0.9.2.5-Darwin.zip")
    logpp_libinc = os.path.join(logpp_build, "lib_logpp-0.9.2.5-Darwin/include")
    logpp_lib = os.path.join(logpp_build, "lib_logpp-0.9.2.5-Darwin/lib/lib_logpp-0.9.2.5.a")
    lib_path = os.path.join(root_path, "lib")
    inc_path = os.path.join(root_path, "include")

    with zipfile.ZipFile(logpp_libzip, 'w') as zip_ref:
        zip_ref.extractall(logpp_build)
    
    if not(os.path.exists(lib_path)):
        os.makedirs(lib_path)
    print("copying " + logpp_lib + " to " + lib_path)
    shutil.copy(logpp_lib, lib_path)

    if not os.path.exists(inc_path):
        os.makedirs(inc_path)
        
    # Iterate over files in logpp_libinc and copy each file to inc_path
    for filename in os.listdir(logpp_libinc):
        src_file = os.path.join(logpp_libinc, filename)
        if os.path.isfile(src_file):  # Only copy files (not directories)
            print("copying " + src_file + " to " + inc_path)
            shutil.copy(src_file, inc_path)
    
    

def main():
    clone_logpp()
    build_logpp()
    cpack_logpp()
    install_logpp()
    print("All done!")


if __name__ == "__main__":
    num_args = len(sys.argv)
    if num_args < 2:
        print("installation root path is needed")
    else:
        root_path = sys.argv[1]
        print(root_path)
        main()


