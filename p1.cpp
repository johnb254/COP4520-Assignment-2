#include <iostream>
#include <cstdlib>
#include <random>
#include <thread>
#include <mutex>

#define GUEST_COUNT 10


// Labyrinth
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

    // Enter the labyrinth
    void enterLabyrinth()
    {
        m.lock();
    }

    // Exit the labyrinth
    void exitLabyrinth()
    {
        m.unlock();
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

    bool isInsideLabyrinth()
    {
        return inLabyrinth;
    }

    // Simulates the strategy a guest should follow after entering the labyrinth
    void enter(Labyrinth *maze)
    {
        maze->enterLabyrinth();
        inLabyrinth = true;

        // If the guest has already entered the labyrinth, they should not ask for the cupcake to be replaced
        if (hasEntered)
        {
            std::cout << "Guest exits the labyrinth" << std::endl;
            maze->exitLabyrinth();
            inLabyrinth = false;
            return;
        }
        // If the guest has not entered the labyrinth, they should take the cupcake if it is at the exit
        if (!(maze->isCupcakeAtExit()))
        {
            maze->replaceCupcake();
            std::cout << "Guest asks for the cupcake to be replaced" << std::endl;
        }
        maze->takeCupcake();
        std::cout << "Guest takes the cupcake" << std::endl;

        std::cout << "Guest exits the labyrinth" << std::endl;
        hasEntered = true;
        maze->exitLabyrinth();
        inLabyrinth = false;
    }
};

int main()
{
    Labyrinth* labyrinth = new Labyrinth();

    Guest *guests = new Guest[GUEST_COUNT];
    std::thread *threads = (std::thread *) malloc(sizeof(std::thread) * 0);

    srand((unsigned)time(NULL));
    int threadCount = 0, noCupcakeCount = 0;
    do
    {
        // Randomly select a guest to enter the labyrinth

        int guestIndex;
        do {
            guestIndex = rand() % GUEST_COUNT;
        } while (guests[guestIndex].isInsideLabyrinth());
        std::cout << "Guest " << guestIndex << " is entering the labyrinth" << std::endl;

        // Create a new thread for the guest
        threads = (std::thread *) realloc(threads, sizeof(std::thread) * (threadCount + 1));
        threads[threadCount] = std::thread(&Guest::enter, &guests[guestIndex], &labyrinth);        threadCount++;

        // Check if the cupcake is at the exit
        if (!labyrinth->isCupcakeAtExit())
        {
            noCupcakeCount++;
        }
        else
        {
            noCupcakeCount = 0;
        }
    } while (noCupcakeCount < GUEST_COUNT); // Repeat until the cupcake is not at the exit for GUEST_COUNT times

    // Wait for all threads to finish
    for (int i = 0; i < threadCount; i++)
    {
        threads[i].join();
    }
    delete[] guests;

    return 0;
}
