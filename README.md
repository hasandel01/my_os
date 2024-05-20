# Setting up Wyoos Development Environment

Before you proceed, make sure that you have downloaded and installed [Xorriso](https://www.gnu.org/software/xorriso/) on your system. Note that Xorriso may not be available by default on some Linux distributions.

## Getting Started

1. **Download and Install Xorriso**: Visit the [Xorriso website](https://www.gnu.org/software/xorriso/) and follow the instructions to download and install Xorriso on your system.

2. **Extract Wyoos Source Code**: After downloading the Wyoos source code, extract it to a directory. Navigate to the extracted folder `/wyoos-0d1984e1f08cc2b0075414cb61e1780e760c6914`.

3. **Open Terminal**: Once you're in the extracted folder, open a terminal window there.

4. **Generate ISO Image**: In the terminal, run the following command:
    ```
    make mykernel.iso
    ```
   This command will generate an ISO image that you can use to run Wyoos on a virtual machine.

## Running on Virtual Machine

You can use any virtual machine software to run Wyoos. For development purposes, VMBox is recommended.

1. **Install VirtualBox**: If you haven't already, download and install [VirtualBox](https://www.virtualbox.org/).

2. **Create a New Virtual Machine**: Open VirtualBox and create a new virtual machine. Follow the prompts, and when prompted for an ISO image, choose the one generated earlier (`mykernel.iso`).

3. **Start the Virtual Machine**: Start the virtual machine to boot into Wyoos.