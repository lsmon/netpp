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
    if not os.path.exists("logpp"):
        return False
    return True


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


def check_latest_version():
    # Check if the local repository is up to date
    local_dir = os.path.join(root_path, "logpp")
    try:
        subprocess.check_call(["git", "remote", "update"], cwd=local_dir)
        subprocess.check_call(["git", "status", "--ahead-behind", "origin/main"], cwd=local_dir)
    except subprocess.CalledProcessError:
        return False
    return True


def update_repository():
    # Update the local repository to the latest version
    local_dir = os.path.join(root_path, "logpp")
    try:
        subprocess.check_call(["git", "pull", "origin", "main"], cwd=local_dir)
    except subprocess.CalledProcessError:
        print("Error updating repository")


def build_logpp():
    print("Building logpp...")

    logpp_path = os.path.join(root_path, "logpp")
    logpp_build = os.path.join(logpp_path, "build")
    os.chdir(logpp_path)
    os.makedirs(logpp_build, exist_ok=True)

    # run_command("cmake -S " + logpp_path + " -B " + logpp_build)
    # run_command("cmake --build " + logpp_build + " -j 14")
    cmake_bin = ""
    if os_type == "darwin": 
        cmake_bin = "/opt/homebrew/bin/cmake"
    else :
        cmake_bin = "cmake"
    print(cmake_bin + " --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S" + logpp_path + " -B" + logpp_build)
    print(cmake_bin + " --build " + logpp_build + " --config Debug --target all -j 12")
    run_command(cmake_bin + " --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S" + logpp_path + " -B" + logpp_build)
    run_command(cmake_bin + " --build " + logpp_build + " --config Debug --target all -j 12")

    print("Building completed.")


def cpack_logpp():
    print("Packing logpp")
    logpp_path = os.path.join(root_path, "logpp")
    logpp_build = os.path.join(logpp_path, "build")
    os.chdir(logpp_build)
    if os_type == "linux":
        cpack = "/usr/bin/cpack"
    elif os_type == "Darwin":
        cpack = "/opt/homebrew/bin/cpack"
    else:
        cpack = "cpack"
    run_command(cpack + " -G ZIP")


def copy_files_and_dirs(src, dst):
    if not os.path.exists(dst):
        os.makedirs(dst)
    for filename in os.listdir(src):
        src_file = os.path.join(src, filename)
        dst_file = os.path.join(dst, filename)
        if os.path.isfile(src_file):
            print(f"copying {src_file} to {dst_file}")
            shutil.copy(src_file, dst_file)
        elif os.path.isdir(src_file):
            copy_files_and_dirs(src_file, dst_file)


def install_logpp():
    logpp_path = os.path.join(root_path, "logpp")
    logpp_build = os.path.join(logpp_path, "build")
    os_postfix = ""
    if os_type == "linux":
        os_postfix = "Linux"
    elif os_type == "darwin":
        os_postfix = "Darwin"
    logpp_libzip = os.path.join(logpp_build, f"lib_logpp-{version}-{os_postfix}.zip")
    logpp_libinc = os.path.join(logpp_build, f"lib_logpp-{version}-{os_postfix}/include")
    logpp_lib = os.path.join(logpp_build, f"lib_logpp-{version}-{os_postfix}/lib/lib_logpp-{version}.a")
    lib_path = os.path.join(root_path, "lib")
    inc_path = os.path.join(root_path, "include")

    with zipfile.ZipFile(logpp_libzip, 'r') as zip_ref:
        zip_ref.extractall(logpp_build)
    
    if not(os.path.exists(lib_path)):
        os.makedirs(lib_path)
    print("copying " + logpp_lib + " to " + lib_path)
    shutil.copy(logpp_lib, lib_path)

    if not os.path.exists(inc_path):
        os.makedirs(inc_path)
        
    # Iterate over files in logpp_libinc and copy each file to inc_path
    print(logpp_libinc)
    copy_files_and_dirs(logpp_libinc, inc_path)
    # for filename in os.listdir(logpp_libinc):
    #     src_file = os.path.join(logpp_libinc, filename)
    #     if os.path.isfile(src_file):  # Only copy files (not directories)
    #         print("copying " + src_file + " to " + inc_path)
    #         shutil.copy(src_file, inc_path)
    
    

def main():
    if not check_cloned():
        clone_logpp()
    else:
        if not check_latest_version():
            update_repository()
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
        version = sys.argv[2]
        print(root_path)
        print(version)
        main()


