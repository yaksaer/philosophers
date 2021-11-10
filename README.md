<h1 align="center">Philosophers - 21 school project</h1>

# Final grade 125/125

Mandatory part: 100/100 \
Bonus: 25/25

## About
This project is a training to multi-threads/multi-process programming with the use of mutex and semaphore.
It contains 2 different programs simulating a twist of the famous Dining Philosophers problem, all with the same basic rules.
This project is also a good lesson in C optimization as we need to save every bit of CPU usage we can to ensure the survival of our philosophers.

philo: multi-threads and use of mutex
philo_bonus: multi-processes and use of semaphore

## Demonstrarion

<p align="center">

<img src="https://github.com/yaksaer/images-vault/blob/master/philovideo.gif?raw=true" >
</p>

## Installation
Compatible MacOS
1. Clone this repo
`git clone git@github.com:yaksaer/philosophers.git`

2. `cd` into the directory and run `make` \
        `cd minishell` \
        `make` or `make bonus` for another project version
## Run the program
After building run `./minishell` from the project root

## Usage

`./philo number_philosopher time_to_die time_to_eat time_to_sleep [number_of_time_each_philosophers_must_eat]` \
time - in milliseconds
arguments in square brackets are not required (number_of_time_each_philosophers_must_eat)

### Example
`./philo 4 410 200 100`\
`4` - number of philosophers \
`410` - time after which philosopher will die without food in milliseconds \
`200` - time which philosopher need for eat \
`100` - time which philosopher need for sleep\

## Philosophers with threads and mutex diagram
<p align="center">

<img src="https://github.com/yaksaer/images-vault/blob/master/philo.png?raw=true" >
</p>
