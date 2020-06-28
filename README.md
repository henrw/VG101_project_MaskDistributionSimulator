# VG101_project_MaskDistributionSimulator
## 1.Timetable
Now - July 1:
i) pre-study c++/OpenGL (try to figure out how to do the parts)
ii) create more details about the program
June 1 - July 5:
i) set up data structure
ii) pre-study c++/OpenGL
July 6 - July 12:
i) write mask distribution function
ii) write infected condition function
iii) plot the map + start to write animation
July 13 - July 19
July 18 Progress report due
i) start to combine the functions in “main”
ii) finish animation
iii) write report
July 20 - July 26
i) wrap things up
ii) user interface beautification
July 27 - August 3 (Due Day)
## 2.Task Assignment
● Jeongsoo Pang:
○ infected condition function (model)
○ user interface beautifying
● Muzhe Wu:
○ data structure creating
○ mask distribution function (model)
● Chenhao Zheng
○ map plotting
○ animation
i) enhancement
## Summary:
The program simulates the redistribution of masks among different cities in
Covid-19 with a certain mask production capacity being given. The redistribution of
masks is carried out based on the situation of infection in a certain city, the number of
masks a certain city can spare, and geometrical factors (e.g. distance in between) all
of which change over time until either the infected number gets down to 0 or there are
not enough masks.
6. Motivation:
In recent months, the disastrous Covid-19 spread around the world, causing a
heavy loss to human beings. To make matters worse, the shortage of medical
resources, like masks, inevitably occurred in the places where the number of infection
grew rapidly. In this case, tons of masks were transported to those places from all
over the country/ world. However, the shortage can not be fully made up unless the
redistribution of mask is fair and wise. For example, initially in Wuhan the
redistribution of mask by the Red Cross is problematic, causing the discontentment
among citizens.
Therefore, we tend to develop of program that can simulate a rapid and
scientific redistribution of masks among different places.
7. Tentative Design
a. The default setting is: there are ten cities in total with the mask distribution
center located in one of them. The initial conditions of the number of infected
people in each city are randomly generated and the user inputs mask
production capacity per day and how many masks there initially are (in total).
A map will be generated with these cities (linked with roads) scattered on it.
The relevant information of each cities will be listed on the right-hand side.
Over time, the situation of infection will change based on the law
(behavior) of contagion and the supply of masks; the masks will be
redistributed based on the situation of infection. (These two factors interact
with each other)
The simulation ends either when the infected number gets down to 0 or
things get out of control (not enough masks).
b. (Probably)
Data structure:
● initial_condition
○ masks_per_day
○ total_num_masks
● city
○ location[2]
○ immediate_num_infected
○ current_num_masks
○ required_num_masks
○ road[n][2]
● truck
○ initial_location[2]
○ final_location[2]
○ speed
● time_string
Building blocks:
● map_plotting
● car_animation
● masks_assumption function
● masks_requiring function
● infected_case_changing function
...
8. Expected Outcome
a. Bottom-line
Overall, we will simulate the arrangement and distribution of masks among
different cities during the COVID-19.
A map of a number (input) of cities will be generated and a legend of key
information of different cities will be provided next to the map. The animation of
trucks over time will be shown on the map.
A data structure will be designed to store the information of cities on:
1. population
2. number of infected people
3. location (coordinates)
4. mask production rate (0 if there's no mask production in this city)
5. the maximum load and speed of trucks which transport masks
After the initialization, the program will start simulate the mask redistribution and
the situation of infection of COVID-19 over time, until either the infected number
gets down to 0 or things get out of control (not enough masks).
Functions of the relation between the number of masks and the infected cases,
how many masks certain cities should give away to another city at a certain time
will be developed.
b. Expected
1. The seriousness of the condition of virus infection or masks will be
demonstrated with different colors.
2. More determinants will be considered in the function of relation between
the number of masks and the infected number.
3. The overall cost of this mask distribution may be considered.
4. A subplot and simulation within a city (freight center-hospital-residence)
may be considered.
5. when the game is over, the curve of the change of infected number and masks
number during the time period can be shown.
c. Potential
1. A game mode may be developed where the player manually decides which
city should give away what number of masks to which city (within a certain
amount of money/time to defeat the virus).
2. A more user-friendly interface/GUI/more mouse-triggered events might be
considered.

10. Extra Preparations
1. We need to learn the common change of infected cases of COVID-19 (curve).
2. Relevant information of transportation fare and production cost need to be
gathered.
