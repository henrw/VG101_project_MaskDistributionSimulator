# VG101_project_MaskDistributionSimulator
## 1.Timetable
before July 1:  
- pre-study c++/OpenGL (try to figure out how to implement our parts)  
- ~~create more details about the program~~
 
June 1 - July 5:  
- set up data structure  
- pre-study c++/OpenGL  

July 6 - July 19: 
**July 18 Progress report due**  
- write mask distribution function
- write infected condition function
- plot the map + start to write animation
- write report  

July 20 - July 26  
- start to combine the functions in \main"
- finish animation
- Start to creat a user interface 

July 27 - **August 3 (Due Day)**  
- enhancement  
## 2.Task Assignment  
- Jeongsoo Pang:  
  - infected condition function (model)  
  - user interface beautifying  
- Muzhe Wu:  
  - data structure creating  
  - mask distribution function (model)  
- Chenhao Zheng  
  - map plotting  
  - animation  

## 3.Summary:
The program simulates redistribution of masks among the cities facing Covid-19 with a certain mask production
capacity. The redistribution of masks is performed based on the seriousness of infection in a city compared to
the number of masks stocked and geometrical factors (e.g. distance between cities). The simulation ends either
when the infected number gets down to 0 or when the number of infection is too large.

## 4.Expected Outcome
### 1. Bottom-line

Overall, we will simulate the arrangement and distribution of masks among dierent cities during the COVID-19.

- A map of a number (input) of cities will be generated and a legend of key information of different cities will
be provided next to the map. The animation of trucks over time will be shown on the map.
- A data structure will be designed to store the information of cities on (listed above).
- After the initialization, the program will start simulate the mask redistribution and the situation of infection
of COVID-19 over time, until either the infected number gets down to 0 or more than 50% people are infected
in a city.
- Functions of the relation between the number of masks and the infected cases, how many masks certain
cities should give away to another city at a certain time will be developed.
### 2. Expected
- The seriousness of the condition of virus infection or masks will be demonstrated with dierent colors.
- More determinants will be considered in the function of relation between the number of masks and the
infected number.
- The overall cost of this mask distribution may be considered.
- When the game is over, the curve of the change of infected number and the number of masks during the
time period can be shown.
### 3. Potential
- A game mode may be developed where the player manually decides which city should give away what
number of masks to which city (within a certain amount of money/time to defeat the virus).
- A more user-friendly interface/GUI/more mouse-triggered events might be considered.

## 5.Extra preparation
1. We need to import data including the infection number of cities in Hubei Province, mask production
rate, numbers of hospitals.
2. We need to study OpenGL and SIR model.
