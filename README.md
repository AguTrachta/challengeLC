# High-Performance Process Manager

This project is a **high-performance process management tool** designed to interactively manage processes, monitor system resources, and log activities. It offers a set of commands to list active processes, monitor system resource usage, kill processes by PID, view recent logs, and more.

You can see the flow diagram [here](https://kroki.io/mermaid/svg/eNptlE1z2jAQhu_5FXtJT3CwSa7tEH-Ap07GE0g7HQ0H1WxBU1liZNE2hf73ypKMDGNu6H1399HuyjtFD3tYp3dgfnOy0lRpqJTcKdpsYDr9CE8kZe2B03f4iryWDcKqVogCPsD8F2WcfucIiWwaKrbtxuZ5soHJKRMaVa_9s1JipZSUrNW9shkoGXmWgmmpxsScfGacjykLUsrdmLAkS-SHMaUg2R82YLDS_T0MySDn8rcVUscdkcLQMcrZX4T1XiHdtvDD0KZUUxPFOdaaSeEKpZGLiolXoCrSCbzQBieQVG8TeMZGqnfvjp17dmm4mUONbWuRvGdmPaW97tyX6slvOhfgM9faEXiopOQudeZgszhMoHqDLo-DBCbgFSmfrlmDPsQRZ7OTbaWPY2L3yQ07c7TnF3k23qujb9ieobygD-cauHM39Y77cNTwxZBvaWhv7ojz-FShaljbGgWSPdY_Xfk8DuXz0NVMKak2Vw5Lkz-QdZdHUI19673twRV6PPWGrtLqWHcWf9f8cVjs6sjf1R75-YVtC2sb7r1wOx1dkF-xRqGt2bwpxdCDLaKbdMNlD_mW7imEfNZWCLO4zaCfy9tswwcSshXu-UQk4UjF8WDoWnlUdQ9V9Gm6P6X7FAyFInYvr__K3P0HJjtExA).


## Project Structure

The project documentation is distributed across multiple `.md` files, each addressing a specific aspect of the project. Below is a summary of these files:

### 1. [INSTALL.md](./INSTALL.md)

This file contains detailed instructions on how to **set up and run** the project both with and without Docker. It includes the following sections:

- **Running with Docker**: Explains how to build and run the Docker container.
- **Running without Docker**: Provides steps to manually set up the environment using Conan, CMake, and other dependencies.

### 2. [CHALLENGE.md](./CHALLENGE.md)

In this file, I explain the **challenge** this project addresses. It provides an overview of the problem space and outlines the specific tasks that the project solves, such as process management, monitoring, and logging. This file also delves deeper into the requeriments of the project.

### 3. [ADDITIONAL_CHALLENGE.md](./ADDITIONAL_CHALLENGE.md)

This file details the **additional challenge** aspect of the project. It builds upon the initial challenge, introducing extra requirements and features that extend the project’s functionality. In this section, we explore the enhancements and improvements that go beyond the core functionality of the process manager.

## Commands Overview

### 1. `list` - List Active Processes

The `list` command displays all active processes running on the system.

#### Example:

```bash
> list
```
This will print out a list of currently running processes, showing details like process IDs (PIDs), CPU usage, and memory usage.

![list](https://github.com/user-attachments/assets/0df88966-238a-448f-af86-22d4e02557e7)

### 2. monitor - Monitor CPU and Memory Usage
The `monitor` command starts a real-time display of the system's CPU and memory usage.

Example:
```bash
> monitor
```
This will show the CPU and memory usage in real-time, updating periodically.

![monitor](https://github.com/user-attachments/assets/50f5a091-e3d3-4b54-bcc0-b9480ff74085)

### 3. kill <pid> - Kill a Process by PID
The `kill` command allows you to terminate a running process by providing its Process ID (PID).

Example:
```bash
> kill 12345
```
This will terminate the process with PID 12345. If no PID is provided, an error message is displayed.

![kill](https://github.com/user-attachments/assets/4a6f68c5-fc06-43ab-9f7b-00c2d96adb2e)

### 4. log - View Recent Logs
The `log` command displays recent log entries related to the application.

Example:
```bash
> log
```
This will show the most recent logs, including any errors or important events that have occurred within the application.

![log](https://github.com/user-attachments/assets/8022de07-024c-4fdb-bce6-9953a13887d8)

