#include <iostream>
#include <cstdlib>
#include <random>
#include <thread>
#include <mutex>

#define GUEST_COUNT 10

class Labyrinth
{
private:
    bool hasCupcake;
    std::mutex m;

public:
    Labyrinth()
    {
        hasCupcake = true;
    }

    // Enter the labyrinth and prevent other guests from entering
    void enterLabyrinth()
    {
        m.lock();
        std::cout << "Guest has entered the labyrinth" << std::endl;
    }

    // Exit the labyrinth and allow other guests to enter
    void exitLabyrinth()
    {
        m.unlock();
        std::cout << "Guest has exited the labyrinth" << std::endl;
    }

    // Replace the cupcake at the exit
    void replaceCupcake()
    {
        hasCupcake = true;
    }

    // Take the cupcake from the exit
    void takeCupcake()
    {
        hasCupcake = false;
    }

    // Check if the cupcake is at the exit
    bool isCupcakeAtExit()
    {
        return hasCupcake;
    }
};

class Guest
{
private:
    bool hasEntered, inLabyrinth;

public:
    Guest()
    {
        hasEntered = false;
        inLabyrinth = false;
    }

    // Check if the guest is inside the labyrinth
    bool isInsideLabyrinth()
    {
        return inLabyrinth;
    }

    // Simulates the strategy a guest should follow after entering the labyrinth
    void enter(Labyrinth *maze)
    {
        maze->enterLabyrinth();
        inLabyrinth = true;

        // If the guest has not entered the labyrinth, they should take the cupcake if it is at the exit
        if (!hasEntered)
        {
            if (!(maze->isCupcakeAtExit()))
            {
                maze->replaceCupcake();
                std::cout << "Guest asks for the cupcake to be replaced" << std::endl;
            }
            maze->takeCupcake();
            std::cout << "Guest takes the cupcake" << std::endl;
        }
        // If the guest has entered the labyrinth, they should replace the cupcake if it is not at the exit
        else
        {
            if (!(maze->isCupcakeAtExit()))
            {
                maze->replaceCupcake();
                std::cout << "Guest asks for the cupcake to be replaced" << std::endl;
            }
        }
        hasEntered = true;
        maze->exitLabyrinth();
        inLabyrinth = false;
    }
};

int main()
{
    Labyrinth *labyrinth = new Labyrinth();

    Guest *guests = new Guest[GUEST_COUNT];

    srand((unsigned)time(NULL));
    
    // Number of consecutive times the cupcake is found at the exit after a guest has gone through the labyrinth
    int cupcakeCount = 0;
    do
    {
        // Randomly select a guest to enter the labyrinth
        int guestIndex;
        do
        {
            guestIndex = rand() % (GUEST_COUNT - 1);
        } while (guests[guestIndex].isInsideLabyrinth());
        std::cout << "Guest " << guestIndex << " is called to enter the labyrinth" << std::endl;

        // Create a new thread for the guest
        std::thread(&Guest::enter, &guests[guestIndex], labyrinth).detach();

        // Check if the cupcake is at the exit
        if (!labyrinth->isCupcakeAtExit())
        {
            cupcakeCount = 0;
        }
        else
        {
            cupcakeCount++;
        }
        std::cout << std::endl;
    } while (cupcakeCount < GUEST_COUNT); // If the cupcake is found at the exit GUEST_COUNT times, all guests have gone through the labyrinth

    std::cout << "All guests have gone through the labyrinth" << std::endl;

    delete[] guests;

    return 0;
}
