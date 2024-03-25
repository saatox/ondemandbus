# Project Title

An Online Algorithm For On-Demand Bus and Patrol Point Scheduling

## Description

As Japan faces changes in its population demographic, the current infrastructures in both urban and rural areas require immediate change. More specifically, many citizens desire an updated transportation system that truly caters to their needs- the majority have no choice but to remain active drivers past the age of 80 just to complete their daily tasks and necessities. The government has put “green-slow-mobility” projects in place to tackle this issue, focusing on cities looking to enhance their commitment to the SDGs. The projects’ main goal is to implement a transportation system in which community members are active participants in a mutually supportive manner. What is important here is the ability of municipalities to maintain this system for many years to come. In order to do so, we have created a system that optimizes the vehicle's schedule based on multiple factors, including but not limited to passenger requests and patrolling of the vicinity. The project created here is in collaboration with a local government, ensuring that the concerns of both the potential passengers and drivers are voiced. 
## Getting Started

### Dependencies

* Runs on most OS's
* Languages used: C, Python3

### Installing

* Modifications may need to be made for the input/output file directories.

### Executing program

1. **Open and run /maps/map.ipynb**  
   This file generates the text file that will be used in the next step, printing the relationships between each bus stop. (File name: dist.txt)  
   The output file created from this program will include the distance matrix for all bus stops, as well as the matrices for all points that are patrolled through when the bus moves from one point to the next.  
   Note that in this project, patrolling includes simply just passing through a point without stopping.

2. **Open and run /FINAL_FILES/text.c**  
   This file generates the text file that will be used in all of the programs in /FINAL_FILES, as the output file will be used as the input.  
   The output file will include all of the information in dist.txt, with the addition of the passengers' requests. The requests are randomized, each passenger having a pick up point (a randomized integer between 0 and 28, as there are 29 potential bus stops to choose from), a drop off point (a randomized integer between 0 and 28 that is **different** from the pick up point selected earlier) and a randomized drop off time (a real number between 0 and 500, as the experiment is set to take place over the course of approximately 8+ hours).  
   The number of passengers can be changed within the program.

3. **Open and run /FINAL_FILES/official.c**  
   This program is the main part of the project, where the optimization process is conducted and its output is the final route that the bus is expected to go through.  
   **Notice:** Change the directory of the input and output files as necessary. 

## Other Files

The other files in /FINAL_FILES/ were used for performance evaluations, and to see which variations of the optimization algorithm worked best considering different criteria. Each version has a separate focus, however the parts that have been altered remain the same: the differences are in the way the cost of the route is calculated, and the values that are prioritized when optimizing the route in the fix_route function. The file graph.ipynb simply visualizes the output of the different files, helping better understand each versions' performance quality.

## Help

If issues occur, check if the necessary changes for the input/output file directory has been made. 

## Authors

Sato Kamimura 

## Version History

* 0.1
    * Initial Release


