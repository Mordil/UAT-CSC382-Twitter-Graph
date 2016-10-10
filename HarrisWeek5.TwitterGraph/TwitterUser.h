#pragma once

#include <vector>
#include <string>

class TwitterUser
{
public:
	/* Creates a new Twitter User with the ID provided.*/
	TwitterUser();
	TwitterUser(std::string& name);
	~TwitterUser() { }

	static void ResetID();

	/* Adds the user provided to their list of followers. */
	void AddFollower(TwitterUser* userToAdd);

	/* Adds the user to the current user's subscription list, and adds the current user to the user provided's followers. */
	void SubscribeToUser(TwitterUser* user);

	/* POC for sending tweets out to subscribers. */
	void Tweet(std::string message);

	/* Scaffolding POC for receiving tweets from people the user is subscribed to. */
	void ReceiveTweet(std::string message);

	/* Does the necessary work to remove the user from the network. */
	void RemoveSelf();
	
	/* Removes the provided node from the current user's subscription list. */
	void RemoveSubscription(TwitterUser* userToRemove);

	/* Removes the provided node from the current user's list of followers. */
	void RemoveFollower(TwitterUser* userToRemove);

	/* Prints out information on the current user. */
	void PrintContents();

	/* Checks if the current user is subscribed to the provided user. */
	bool CheckIsSubscribedTo(TwitterUser* user);

	/* Returns the user's ID. */
	const int& GetID() { return _id; }

	/* Creates a friendly display string representing the user. */
	const std::string ToString() const;

	/* Returns the user's name. */
	const std::string& GetName() { return _name; }

	/* Returns the user's Followers list. */
	const std::vector<TwitterUser*>& GetFollowers() { return _listOfFollowers; }
	/* Returns the user's subscription list. */
	const std::vector<TwitterUser*>& GetSubscriptions() { return _subscriptionList; }

private:
	static int autoIncrementId;

	int _id;

	std::string _name;

	std::vector<TwitterUser*> _listOfFollowers;
	std::vector<TwitterUser*> _subscriptionList;

	std::string _buildPrintStringForList(std::vector<TwitterUser*>& list);
};

