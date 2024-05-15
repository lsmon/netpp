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


def clone_json():
    print("Cloning json...")
    os.chdir(root_path)
    # Clone or update nlohmann/json using Git (you need Git installed)
    if not os.path.exists("json"):
        run_command("git clone https://github.com/lsmon/json.git json")
    else:
        os.chdir("json")
        run_command("git pull origin main")
        os.chdir("..")
    print("Cloning completed.")


def build_json():
    print("Building json...")

    json_path = os.path.join(root_path, "json")
    json_build = os.path.join(json_path, "cmake-build-debug")
    os.chdir(json_path)
    os.makedirs(json_build, exist_ok=True)

    os_name = platform.system()
    print(os_name)
    run_command("cmake -S " + json_path + " -B " + json_build)
    run_command("cmake --build " + json_build + " -j 14")
    print("Building completed.")


def cpack_json():
    print("Packing json")
    json_path = os.path.join(root_path, "json")
    json_build = os.path.join(json_path, "cmake-build-debug")
    os.chdir(json_build)
    run_command("cpack -C Debug")


def install_json():
    print(os.name)
    json_path = os.path.join(root_path, "json")
    json_build = os.path.join(json_path, "cmake-build-debug")
    json_libzip = os.path.join(json_build, "lib_json-0.2.1.1-Darwin.zip")
    json_libinc = os.path.join(json_build, "lib_json-0.2.1.1-Darwin/include")
    json_lib = os.path.join(json_build, "lib_json-0.2.1.1-Darwin/lib/lib_json-0.2.1.1.a")
    lib_path = os.path.join(root_path, "lib")
    inc_path = os.path.join(root_path, "include")

    with zipfile.ZipFile(json_libzip, 'w') as zip_ref:
        zip_ref.extractall(json_build)
    
    if not(os.path.exists(lib_path)):
        os.makedirs(lib_path)
    print("copying " + json_lib + " to " + lib_path)
    shutil.copy(json_lib, lib_path)

    if not os.path.exists(inc_path):
        os.makedirs(inc_path)
        
    # Iterate over files in json_libinc and copy each file to inc_path
    for filename in os.listdir(json_libinc):
        src_file = os.path.join(json_libinc, filename)
        if os.path.isfile(src_file):  # Only copy files (not directories)
            print("copying " + src_file + " to " + inc_path)
            shutil.copy(src_file, inc_path)
    
    

def main():
    clone_json()
    build_json()
    cpack_json()
    install_json()
    print("All done!")


if __name__ == "__main__":
    num_args = len(sys.argv)
    if num_args < 2:
        print("installation root path is needed")
    else:
        root_path = sys.argv[1]
        print(root_path)
        main()


