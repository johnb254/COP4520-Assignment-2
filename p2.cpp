#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <random>

using namespace std;

#define GUEST_COUNT 1000

class Showroom
{
private:
    mutex m;

public:
    // Ensure that only one guest can enter the showroom at a time
    bool enterShowroom()
    {
        // Guest tries to enter the showroom
        if (!m.try_lock()) 
        {
            // Sign says "BUSY", guest leaves
            cout << "Showroom is occupied" << endl;
            return false;
        }
        // Guest flips the sign to "BUSY"
        cout << "Guest has entered the showroom" << endl;

        // Guest spends a random amount of time in the showroom
        srand((unsigned)time(NULL));
        this_thread::sleep_for(chrono::milliseconds(rand() % 100));

        // Guest flips the sign to "AVAILABLE"
        m.unlock();
        return true;
    }
};

class Guest
{
private:
    bool hasEntered;

public:
    Guest()
    {
        hasEntered = false;
    }

    void enterShowroom(Showroom &s)
    {
        do
        {
            hasEntered = s.enterShowroom();

            // Randomly wait for 0-300ms before trying again
            srand((unsigned)time(NULL));
            this_thread::sleep_for(chrono::milliseconds(rand() % 300));
        } while (!hasEntered); // Keep trying until the guest has entered the showroom
    }
};

int main()
{
    Showroom s = Showroom();
    Guest guests[GUEST_COUNT];

    thread t[GUEST_COUNT];

    // Create GUEST_COUNT threads, each representing a guest
    for (int i = 0; i < GUEST_COUNT; i++)
    {
        t[i] = thread(&Guest::enterShowroom, &guests[i], ref(s));
    }

    // Wait for all guests to finish
    for (int i = 0; i < GUEST_COUNT; i++)
    {
        t[i].join();
    }

    cout << "All guests have visited the showroom" << endl;

    return 0;
}
