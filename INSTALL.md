## Running with Docker

### Build the Docker Image:

Make sure you have Docker installed and running. Navigate to your project directory and build the Docker image using the following command:

```bash
docker build -t process_manager_image .
```
This command will build the Docker image based on the Dockerfile in the root of the project.

### Run the Docker Container:

After building the Docker image, you can run the container interactively:

```bash
docker run -it process_manager_image /bin/bash
```
This will start the container. Then, run:

```bash
./build/process_manager
```

## Running Without Docker

If you prefer to run the project without Docker, follow these steps to set up the environment manually.

### Prerequisites

Make sure you have the following tools installed on your system:

- **Python 3.x** (with `pip`)
- **CMake** (for building the project)
- **Make** (for compiling the project)
- **G++/GCC** (C++ compiler)

### 1. Install Conan (Python Dependency Manager)

Create a virtual environment for the project and activate it:

```bash
python3 -m venv myenv
source myenv/bin/activate
```
Next, install Conan using pip:

```bash
pip install conan
```
### 2. Prepare the Build Directory
In the project root directory (process_manager), create a build directory:

```bash
mkdir build
cd build
```
### 3. Install Dependencies with Conan
Inside the build directory, run the following command to install the dependencies defined in conanfile.txt:

```bash
conan install .. --build=missing
```
This will install the necessary dependencies and create the required files for building the project.
Once you're done, you can deactivate the virtual environment:

```bash
deactivate
```

### 4. Configure the Project with CMake
After installing the dependencies, use CMake to configure the project:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```
This will configure the build system for the Release version.

### 5. Build the Project
Once the project is configured, you can build it using make:

```bash
make
```
6. Running the Executables
After building the project, you can run either the process_manager executable or the resource_test executable.

To run the process_manager:

```bash
./process_manager
```
To run the resource_test:

```bash
./resource_test
```
