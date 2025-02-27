//Game.dat --> number of tokens --> lab3
//Player A and player B 3 threads each
//Thread --> which player it belongs, and number of tokens to remove
// **** To make it simpler, for each player there will be a thread that decreases the counter by 1, another by 2, and another one by 3.

//Starts player A
    // 1. thre threads lock file
    // 2. But only one of them (which, from our perspective, will be random) will succeed, and will decrease the number of tokens in game.dat according to its value.
    // 3. pass turn --> Make proper use of synchronization to ensure that only one thread of one player modifies the file at a time. 
    //Update game.dat!!!!!!!!!
//Game ends --> ll threads should be woken up and declare whether player A or B has won. Then, make sure that the program ends correctly.

    // CODE GIVEN --> requires all the synchronization mechanisms to make the game work as described.
//IMPORTANT --> make sure to avoid active waiting and starvation (HINT: use condition variables). 