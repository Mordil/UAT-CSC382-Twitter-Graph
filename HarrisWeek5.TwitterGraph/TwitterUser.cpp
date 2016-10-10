#include <iostream>

#include "TwitterUser.h"
#include "Utility.h"

int TwitterUser::autoIncrementId = 1;

TwitterUser::TwitterUser()
{
	_id = autoIncrementId;
	_name = std::to_string(_id);
	autoIncrementId++;
}

TwitterUser::TwitterUser(std::string& name) : TwitterUser() { _name = name; }

void TwitterUser::ResetID()
{
	autoIncrementId = 1;
}

void TwitterUser::AddFollower(TwitterUser* userToAdd)
{
	if (std::find(_listOfFollowers.begin(), _listOfFollowers.end(), userToAdd) == _listOfFollowers.end())
	{
		_listOfFollowers.push_back(userToAdd);
	}
}

void TwitterUser::SubscribeToUser(TwitterUser* user)
{
	if (std::find(_subscriptionList.begin(), _subscriptionList.end(), user) == _subscriptionList.end())
	{
		_subscriptionList.push_back(user);
		user->AddFollower(this);
	}
}

void TwitterUser::Tweet(std::string message)
{
	for (std::vector<TwitterUser*>::const_iterator i = _listOfFollowers.begin(); i != _listOfFollowers.end(); ++i)
	{
		(*i)->ReceiveTweet(message);
	}
}

void TwitterUser::ReceiveTweet(std::string message) { }

void TwitterUser::RemoveSelf()
{
	for (TwitterUser* node : _listOfFollowers)
	{
		node->RemoveSubscription(this);
	}

	for (TwitterUser* node : _subscriptionList)
	{
		node->RemoveFollower(this);
	}
}

void TwitterUser::RemoveSubscription(TwitterUser* userToRemove)
{
	removeUserFromVector(userToRemove, _subscriptionList);
}

void TwitterUser::RemoveFollower(TwitterUser* userToRemove)
{
	removeUserFromVector(userToRemove, _listOfFollowers);
}

void TwitterUser::PrintContents()
{
	std::string stringToPrint = ("-- " + _name + " --\n");

	// add ID
	stringToPrint += ("ID:\t" + std::to_string(_id) + "\n");

	// if the subscription list is small enough and the list not empty, print a short line for each
	if (_subscriptionList.size() < 5 &&
		!_subscriptionList.empty())
	{
		stringToPrint += "\nSubscribed to:\n";
		stringToPrint += _buildPrintStringForList(_subscriptionList);
	}
	// otherwise just print the count
	else
	{
		stringToPrint += "\nNumber of people subscribed to: " + std::to_string(_subscriptionList.size()) + "\n";
	}

	// if the follower count is small enough and the list not empty, print a short line for each
	if (_listOfFollowers.size() < 5 &&
		!_listOfFollowers.empty())
	{
		stringToPrint += "\nFollowers:\n";
		stringToPrint += _buildPrintStringForList(_listOfFollowers);
	}
	// otherwise just print the count
	else
	{
		stringToPrint += "\nNumber of followers: " + std::to_string(_listOfFollowers.size());
	}

	stringToPrint += "\n\n";
	std::cout << stringToPrint;
}

bool TwitterUser::CheckIsSubscribedTo(TwitterUser* user)
{
	std::vector<TwitterUser*>::iterator it = std::find(_subscriptionList.begin(), _subscriptionList.end(), user);

	return it != _subscriptionList.end();
}

const std::string TwitterUser::ToString() const
{
	return _name + " (" + std::to_string(_id) + ")";
}

std::string TwitterUser::_buildPrintStringForList(std::vector<TwitterUser*>& list)
{
	std::string str;

	for (TwitterUser* node : list)
	{
		str += node->GetName() + " (ID: " + std::to_string(node->GetID()) + ")\n";
	}

	return str;
}
