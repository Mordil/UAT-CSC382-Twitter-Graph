#include <iostream>
#include <cstdlib>

#include "TwitterGraph.h"
#include "Utility.h"

using namespace std;

Timer TIMER;

int getRandomID(TwitterGraph& graph)
{
	srand(time(NULL));
	return rand() % graph.GetNumberOfUsers() + 1;
}

int getRandomID(TwitterGraph& graph, int idToSkip)
{
	int retVal = idToSkip;

	while (retVal == idToSkip)
	{
		retVal = getRandomID(graph);
	}

	return retVal;
}

void buildStaticGraph(TwitterGraph& graph)
{
	TwitterUser::ResetID();

	TwitterUser* user1 = graph.AddUser("Donald Trump");
	TwitterUser* user2 = graph.AddUser("Hillary Clinton");
	TwitterUser* user3 = graph.AddUser("George Washington");
	TwitterUser* user4 = graph.AddUser("James Madison");
	TwitterUser* user5 = graph.AddUser("Thomas Jefferson");
	TwitterUser* user6 = graph.AddUser("Samuel Adams");
	TwitterUser* user7 = graph.AddUser("Ulysses Grant");
	TwitterUser* user8 = graph.AddUser("Bill Clinton");
	TwitterUser* user9 = graph.AddUser("George W. Bush");
	TwitterUser* user10 = graph.AddUser("Alexander Hamilton");
	TwitterUser* user11 = graph.AddUser("Ronald Reagan");

	user1->SubscribeToUser(user2);
	user1->SubscribeToUser(user3);

	user2->SubscribeToUser(user3);
	user2->SubscribeToUser(user6);

	user3->SubscribeToUser(user4);
	user3->SubscribeToUser(user5);

	user4->SubscribeToUser(user1);
	user4->SubscribeToUser(user3);

	user5->SubscribeToUser(user3);
	user5->SubscribeToUser(user6);
	user5->SubscribeToUser(user7);

	user7->SubscribeToUser(user5);

	user8->SubscribeToUser(user6);
	user8->SubscribeToUser(user5);
	user8->SubscribeToUser(user7);
	user8->SubscribeToUser(user9);

	user9->SubscribeToUser(user11);

	user11->SubscribeToUser(user3);
}

void buildGraph(TwitterGraph& graph, int numberOfUsers)
{
	TwitterUser::ResetID();

	// add the users
	for (int i = 0; i < numberOfUsers; ++i)
	{
		graph.AddUser();
	}

	vector<TwitterUser*> userList = graph.GetUsers();

	srand(time(NULL));

	// create their associations
	for (vector<TwitterUser*>::iterator i = userList.begin(); i != userList.end(); ++i)
	{
		int chance = rand() % 100 + 1;

		// 10% of isolated users
		if (chance > 10)
		{
			int graphUserCount = graph.GetNumberOfUsers();
			// get the number of people to associate with the current user by getting a random number between the count of users and 2, halved. (2/2 = 1)
			int numberOfPeople = (rand() % graphUserCount + 2) / 2;
			TwitterUser* currentUser = *i;

			// 30% chance user is only followed
			if (chance <= 30)
			{
				for (int j = 0; j < numberOfPeople; ++j)
				{
					int userIdToFind = getRandomID(graph, currentUser->GetID());
					TwitterUser* user = graph.FindUser(userIdToFind);
					user->SubscribeToUser(currentUser);
				}
			}
			// 50% chance user is only subscribed
			else if (chance <= 50)
			{
				for (int j = 0; j < numberOfPeople; ++j)
				{
					int userIdToFind = getRandomID(graph, currentUser->GetID());
					TwitterUser* user = graph.FindUser(userIdToFind);
					currentUser->SubscribeToUser(user);
				}
			}
			// all other scenarios the user is both subscribed and followed
			else
			{
				for (int j = 0; j < numberOfPeople; ++j)
				{
					int userIdToFind = getRandomID(graph, currentUser->GetID());
					TwitterUser* user = graph.FindUser(userIdToFind);
					currentUser->SubscribeToUser(user);
				}

				for (int j = 0; j < numberOfPeople; ++j)
				{
					int userIdToFind = getRandomID(graph, currentUser->GetID());
					TwitterUser* user = graph.FindUser(userIdToFind);
					user->SubscribeToUser(currentUser);
				}
			}
		}
	}
}

void distanceSimulation(TwitterGraph& graph, int& simulationCount)
{
	TwitterUser* user1 = graph.FindUser(getRandomID(graph));
	TwitterUser* user2 = graph.FindUser(getRandomID(graph, user1->GetID()));

	TIMER.Start();
	int distance = graph.DistanceBetweenNodes(user1, user2);
	std::chrono::duration<double, milli> time = TIMER.Stop();

	switch (distance)
	{
		case -10:
			cout << "Simulation " << simulationCount << " (" << time.count() << " ms): No possible connection between user ID " << user1->GetID() << " and user ID " << user2->GetID() << ".\n";
			break;

		case 0:
			cout << "Simulation " << simulationCount << " (" << time.count() << " ms): User ID " << user1->GetID() << " and user ID " << user2->GetID() << " are in the same network.\n";
			break;

		default:
			string plural = (distance > 1) ? "nodes" : "node";
			cout << "Simulation " << simulationCount << " (" << time.count() << " ms): User ID " << user1->GetID() << " is " << distance << " " << plural << " away from user ID " << user2->GetID() << ".\n";
			break;
	}
}

void staticDistanceSimulation(TwitterGraph& graph, TwitterUser* first, TwitterUser* second)
{
	TIMER.Start();
	int distance = graph.DistanceBetweenNodes(first, second);
	std::chrono::duration<double, milli> time = TIMER.Stop();

	switch (distance)
	{
		case -10:
			cout << "(" << time.count() << " ms): No possible connection between user ID " << first->GetID() << " and user ID " << second->GetID() << ".\n";
			break;

		case 0:
			cout << "(" << time.count() << " ms): User ID " << first->GetID() << " and user ID " << second->GetID() << " are in the same network.\n";
			break;

		default:
			string plural = (distance > 1) ? "nodes" : "node";
			cout << "(" << time.count() << " ms): User ID " << first->GetID() << " is " << distance << " " << plural << " away from user ID " << second->GetID() << ".\n";
			break;
	}
}

void simulateGraph(int numberOfUsers)
{
	cout << "Building new Twitter Graph with " << numberOfUsers << " users...\n";

	TwitterGraph graph;

	TIMER.Start();

	buildGraph(graph, numberOfUsers);

	std::chrono::duration<double> time = TIMER.Stop();
	cout << "Finished building Twitter Graph with " << numberOfUsers << " users. (Took " << time.count() << " seconds)\n\n";

	graph.Print();

	cout << "-- Distance Simulations --\n";
	for (int i = 1; i <= 5; ++i)
	{
		distanceSimulation(graph, i);
	}

	cout << "\n\n";
}

int main()
{
	// Uncomment these lines to see more dynamic graphs to be calculated
	// Building is an exponential time complexity, so I don't recommend the 2500 graph.
	/*simulateGraph(5);
	simulateGraph(10);
	simulateGraph(50);
	simulateGraph(100);
	simulateGraph(500);
	simulateGraph(1000);*/
	//simulateGraph(2500);

	// this builds a static graph to use as a reference point to verify the algorithm works
	TwitterGraph graph;
	buildStaticGraph(graph);

	TwitterUser* user1 = graph.FindUser(1);
	TwitterUser* user2 = graph.FindUser(9);

	staticDistanceSimulation(graph, user1, user2);

	user2 = graph.FindUser(8);
	staticDistanceSimulation(graph, user1, user2);

	user2 = graph.FindUser(6);
	staticDistanceSimulation(graph, user1, user2);

	user2 = graph.FindUser(4);
	staticDistanceSimulation(graph, user1, user2);

	user2 = graph.FindUser(10);
	staticDistanceSimulation(graph, user1, user2);

	cout << "\n\n";

	system("pause");
}
