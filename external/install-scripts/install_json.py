import os
import platform
import subprocess
import sys
import zipfile
import shutil

root_path = ""
version = ""
os_type = sys.platform

def run_command(command):
    result = subprocess.run(command, shell=True)
    if result.returncode != 0:
        raise Exception(f"Command '{command}' failed with exit code {result.returncode}.")


def check_cloned():
    # Check if the repository has been cloned
    os.chdir(root_path)
    if not os.path.exists("json"):
        return False
    return True


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


def check_latest_version():
    # Check if the local repository is up to date
    local_dir = os.path.join(root_path, "json")
    try:
        subprocess.check_call(["git", "remote", "update"], cwd=local_dir)
        subprocess.check_call(["git", "status", "--ahead-behind", "origin/main"], cwd=local_dir)
    except subprocess.CalledProcessError:
        return False
    return True


def update_repository():
    # Update the local repository to the latest version
    local_dir = os.path.join(root_path, "json")
    try:
        subprocess.check_call(["git", "pull", "origin", "main"], cwd=local_dir)
    except subprocess.CalledProcessError:
        print("Error updating repository")


def build_json():
    print("Building json...")

    json_path = os.path.join(root_path, "json")
    json_build = os.path.join(json_path, "build")
    os.chdir(json_path)
    os.makedirs(json_build, exist_ok=True)

    os_name = platform.system()
    print(os_name)
    print("cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S" + json_path + " -B" + json_build)
    print("cmake --build " + json_build + " --config Debug --target all -j 12 --")
    cmake_bin = "";
    if os_name == "Darwin": 
        cmake_bin = "/opt/homebrew/bin/cmake"
    else :
        cmake_bin = "cmake"
    
    run_command((cmake_bin + " --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S" + json_path + " -B" + json_build))
    run_command(cmake_bin + " --build " + json_build + " --config Debug --target all -j 12 --")
    print("Building completed.")


def cpack_json():
    print("Packing json")
    json_path = os.path.join(root_path, "json")
    json_build = os.path.join(json_path, "build")
    os.chdir(json_build)
    run_command("cpack -G ZIP")


def install_json():
    print(os.name)
    json_path = os.path.join(root_path, "json")
    json_build = os.path.join(json_path, "build")
    os_postfix = ""
    if os_type == "linux":
        os_postfix = "Linux"
    elif os_type == "darwin":
        os_postfix = "Darwin"
    json_libzip = os.path.join(json_build, f"lib_json-{version}-{os_postfix}.zip")
    json_libinc = os.path.join(json_build, f"lib_json-{version}-{os_postfix}/include")
    json_lib = os.path.join(json_build, f"lib_json-{version}-{os_postfix}/lib/lib_json-{version}.a")
    lib_path = os.path.join(root_path, "lib")
    inc_path = os.path.join(root_path, "include")

    with zipfile.ZipFile(json_libzip, 'r') as zip_ref:
        zip_ref.extractall(json_build)
    
    if not(os.path.exists(lib_path)):
        os.makedirs(lib_path)
    print("copying " + json_lib + " to " + lib_path)
    shutil.copy(json_lib, lib_path)

    if not os.path.exists(inc_path):
        os.makedirs(inc_path)
    # print("copying " + json_libinc + " to " + inc_path)
    # shutil.copy_folder(json_libinc, inc_path)

    # Iterate over files in json_libinc and copy each file to inc_path
    for filename in os.listdir(json_libinc):
        src_file = os.path.join(json_libinc, filename)
        
        if os.path.isfile(src_file):  # Only copy files (not directories)
            print("copying " + src_file + " to " + inc_path)
            # inc_json_path = os.path.join(inc_path, "json")
            if not os.path.exists(inc_path):
                os.makedirs(inc_path)
            shutil.copy(src_file, inc_path)

    json_libinc = os.path.join(json_libinc, "util")
    inc_path = os.path.join(inc_path, "util")
    # Iterate over files in json_libinc and copy each file to inc_path
    for filename in os.listdir(json_libinc):
        src_file = os.path.join(json_libinc, filename)
        
        if os.path.isfile(src_file):  # Only copy files (not directories)
            print("copying " + src_file + " to " + inc_path)
            if not os.path.exists(inc_path):
                os.makedirs(inc_path)
            shutil.copy(src_file, inc_path)
    
    

def main():
    if not check_cloned():
        clone_json()
    else:
        if not check_latest_version():
            update_repository()
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
        version = sys.argv[2]
        print(root_path)
        print(version)
        main()


