
# Package Manager Options

## 1-Summary

This branch's purpose is to explain the function of pakcage managers and analyze the current state of affairs for which package manager the team should use. Major candidates considered include PlatformIO and Conan. Both of these expansive management systems have been examined as the easiet package managers to cooperate with and the most comprehendable for new team members.

## 2-General Purpose of Package Managers

The purpose of package managers are to compile and store libraries and the information stored in libraries in an organized way, so when code is being made, the package managers can easily identify and recall information stored in those downloaded libraries.

## 3-Current System

The current package manager this team uses is PlatformIO. PlatformIO enables coders to communicate efficiently with the various electrical systems in the car.

Two of the main modes we need to communicate with are the ESP32 microcontroller and the CAN bus (control area network bus). When combined properly, these hardwares will send instructions to each other, allowing there to be a flow of information between the car's mechanical and electrical systems.

In order to make this work, we must build programs to hand those instructions to the proper hardware in the car. One way we can build these programs efficiently is through package managers.

When using several different libraries, it may become overwhelming to navigate through all of the syntax and functions stored within, especially when working on bigger projects like this one. With package managers like PlatformIO, we can find the precise function we need under the organization of one program, rather than sifting through separate libraries to find the correct function.

PlatformIO is one of the easiest package managers to understand, as many tools are included in this program to help the user understand and get the most out of their code.
Features like toolchain management (for using tools like debugging and framework applications like Arduino), library pulling from a registry or a custom source, and keeping programs uniform among Linux, Mac, and Windows users.

PlatformIO is also more friendly to beginners in coding, since it provides various tools to build a decent foundation of knowledge in order to effectively build the electrical systems, which is one main reason why PlatformIO has been the package manager for this racing team.

## 4-Possible Package Managers Going Forward

Below are some possible package managers to use to prepare a better car, while keeping in mind future rule and modification changes that may affect how we configure the electrical systems of the car.

### 4.1-PlatformIO

PlatformIO supports Mac, Windows, and Linux operating systems.

#### 4.1.1-Experimentation

Because PlatformIO has a direct extension in VS Code, it is already extremely easy to work with. In addition, because PlatformIO is embedded into all of the systems the car needs, like Arduino boards and the ESP32, the team already knows that PlatformIO is completely viable.

PlatformIO allows users to have a little more control over what libraries are seen and used, while recording all of the vital code in a separatem, but easily accessible location.

PlatformIO is a little more intuitive, due to its extremely organized nature, which beginners can profit from and cultivate a solid foundation of coding knowledge.

#### 4.1.2-Pros/Cons

Pros:

1. PlatformIO is what the team is currently using, because it hooks up nicely to the Arduino and ESP32 systems, and it places an ephasis on using CAN libraries to draw variables from and make code.

2. Communicates very well between all of the electrical and mechanical inputs of the car.

3. Provides excellent resources, like a debugger, to help coders improve the funcitonality and design of their code.

Cons:

1. If the project gets bigger, then there is a greater risk that the car will need to run with a different package manager, in order to be able to store all of the libraries that the car will use.

2. The OS is limited to only a few operating systems like Mac, Windows, and Linux.

3. The team will have to keep the car a little more simple, only because PlatformIO may have complications in the future, if the same variables pop up as different output values in separate libraries.

### 4.2-Conan

Conan supports Mac, Windows, and Linux operating systems.

#### 4.2.1-Experimentation

As a beginner, this package manager was difficult to navigate and comprehend. When the package manager did work, however, there were plenty of libraries to access.

From these libraries, users can access various sorts of libraries including, but not limited to libraries about security, math, basic C++ utilities, and data.

Many of the libraries can be installed through terminal commands in VS Code, which was presented as a struggle at first, because this was not a straightforward process. However, if one attempts to use Google to figure out how to properly download libraries using Conan, then most step-by-step processes found will yield the correct method of downloading libraries.

Conan does not include very many toolchains, and just like everything else, these toolchains must be downloaded manually.

#### 4.2.2-Pros/Cons

Pros:

1. Operates very closely to Python's pip

2. Supports targeting ARM and works well with embedded systems

3. Provides a high-quality performance for handling dependancy graphs

Cons:

1. Beneficial toolchains like a debugger are not present

2. Needs a deeper level of knowledge and understanding to fully maximize the package manager's potential

3. May be overkill if users are only using smaller libraries or are handling very few libraries at all

## 5-The Prospective Package Manager

The most plausible package manager for this team to use in the future is PlatformIO.

### 5.1-Reasoning

PlatformIO is very useful for this team's current circumstances.

1. The amount of information that the car needs to process does not exceed the resources that PlatformIO provides. If this team were to use Conan, there would need to be a stronger case in favor of using a bigger database to pull variables from.

2. Concerning the skill level needed to use this system under some power of knowledge, PlatformIO is more intutive than the rest of the package managers. Because of features like toolchains and frameworks, PlatformIO takes off plenty of the burden of installing different coding tools, making it easier for users to fix their code without going through too much hassle.

3. PlatformIO connects very well to the ESP and Arduino boards that are already in use. PlatformIO also supports CAN communication with the boards, so it would be easier to set up all of our code this way, rather than redo all of the communication through Conan.

### 5.2-Installation
---------Step 1: Make sure VS Code is installed

Download from https://code.visualstudio.com and follow the instructions for the correct operating system.

---------Step 2: Install the PlatformIO IDE extension

In VS Code, click the Extensions icon on the left sidebar.

In the search bar, type "PlatformIO IDE" and find the one published by PlatformIO Labs, then click install.

---------Step 3: Wait for automatic setup

This will install the tools included in all of the PlatformIO software.

---------Step 4: Verify installation

Click the alien-head icon and you should see the PlatformIO home screen with a button for New Project with a bottom status bar showing “PlatformIO Ready”.

---------Troubleshooting

If VS Code says "pio command not found", then restart VS Code after installing it. If it still does not work, then restart the whole computer.

If VS Code says "Python not found" or if PlatformIO is blocked, then install Python manually from python.org and follow the instructions for your operating system.

## 6-Conclusion

Package managers are available to aid coders by presenting fluid, strong, and reliable resources under libraries to ensure solid and consistent communication between the different systems of the car.

PlatformIO will be the best package manager for the present moment, because its realiable and easy-to-understand platform presents an extremely cooperative software to learn about the car's code, to connect the code with physical boards, and to understand all of the mechanisms of the car.